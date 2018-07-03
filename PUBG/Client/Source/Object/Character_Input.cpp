#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "Item.h"
#include "ItemInfo.h"

void Character::setAttacking()
{
    //for Debug =========================================================
    Item* hand = m_totalInventory.m_hand;
    if (hand)
    {
        TAG_RES_STATIC tagDebug = hand->GetTagResStatic();
        Debug << "On hand Weapon: ";
        Debug << ItemInfo::GetName(tagDebug) << "\n";
    }
    Debug << "Attacking: " << ForDebugGetAttacking(m_attacking) << "\n";
    //===================================================================

    TotalInventory& inven = m_totalInventory;

    if (m_pAnimation->HasUpperFinishEvent()) return;

    if (m_currentOnceKey._Num1)
    {
        if (inven.m_weaponPrimary)
        {
            if (m_attacking == Attacking::Unarmed)
            {
                m_attacking = Attacking::Rifle;

                inven.m_hand = inven.m_weaponPrimary;
                inven.m_weaponPrimary = nullptr;
                setRifleOnHand(TAG_RIFLE::Primary);
            }
            else if (m_attacking == Attacking::Rifle) //보조무기를 해제하고, 주무기를 장착한다
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::UPPER,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static,
                    false, [this, &inven]()
                {
                    inven.m_weaponSecondary = inven.m_hand;
                    inven.m_hand = inven.m_weaponPrimary;
                    inven.m_weaponPrimary = nullptr;
                    setRifleOnHand(TAG_RIFLE::Primary);
                });
            }
            else if (m_attacking == Attacking::Melee)
            {
                //TODO: melee 생기면 구현
            }
        }
    }
    else if (m_currentOnceKey._Num2)
    {
        if (inven.m_weaponSecondary)
        {
            if (m_attacking == Attacking::Unarmed)
            {
                m_attacking = Attacking::Rifle;

                inven.m_hand = inven.m_weaponSecondary;
                inven.m_weaponSecondary = nullptr;
                setRifleOnHand(TAG_RIFLE::Secondary);
            }
            else if (m_attacking == Attacking::Rifle) //주무기를 등짝에 붙이고 보조무기를 손에 든다
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::UPPER,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static,
                    false, [this, &inven]()
                {
                    inven.m_weaponPrimary = inven.m_hand;
                    inven.m_hand = inven.m_weaponSecondary;
                    inven.m_weaponSecondary = nullptr;
                    setRifleOnHand(TAG_RIFLE::Secondary);
                });
            }
            else if (m_attacking == Attacking::Melee)
            {
                //TODO: melee 생기면 구현
            }
        }
    }
    else if (m_currentOnceKey._X)
    {
        //무기가 주무기냐, 보조무기냐에 따라서 다른 애니메이션을 실행한다. 우선 QBZ 주무기 Kar98k 보조무기
        //등에 부착한다
        if (inven.m_hand)
        {
            m_attacking = Attacking::Unarmed;
            inven.m_tempSaveWeaponForX = inven.m_hand;
            TAG_RES_STATIC tag = inven.m_hand->GetTagResStatic();

            if (tag == TAG_RES_STATIC::QBZ)
                setRifleOnBody(TAG_RIFLE::Primary);
            else if (tag == TAG_RES_STATIC::Kar98k)
                setRifleOnBody(TAG_RIFLE::Secondary);
        }
        //손에 무기를 들고 있지않는데 X버튼을 누른다면, 이전에 장착했던 무기를 다시 손에 든다
        else
        {
            if (inven.m_tempSaveWeaponForX)
            {
                m_attacking = Attacking::Rifle;
                inven.m_hand = inven.m_tempSaveWeaponForX;
                inven.m_tempSaveWeaponForX = nullptr;
                TAG_RES_STATIC tag = inven.m_hand->GetTagResStatic();

                if (tag == TAG_RES_STATIC::QBZ)
                {
                    inven.m_weaponPrimary = nullptr;
                    setRifleOnHand(TAG_RIFLE::Primary);
                }
                else if (tag == TAG_RES_STATIC::Kar98k)
                {
                    inven.m_weaponSecondary = nullptr;
                    setRifleOnHand(TAG_RIFLE::Secondary);                
                }
            }
        }
    }
}

void Character::setStance()
{
    //for Debug
    Debug << "Stance: " << ForDebugGetStance(m_stance) << "\n";

    if (m_currentOnceKey._C)
    {
        if (m_stance == Stance::Crouch)
        {
            // TODO : check obstacle

            m_stance = Stance::Stand;
        }
        else
        {
            // TODO : check obstacle

            m_stance = Stance::Crouch;
        }
    }
    else if (m_currentOnceKey._Z)
    {
        if (m_stance == Stance::Prone)
        {
            // TODO : check obstacle

            m_stance = Stance::Stand;
        }
        else
        {
            // TODO : check obstacle

            m_stance = Stance::Prone;
        }
    }
}

void Character::setReload()
{
    TotalInventory& inven = m_totalInventory;

    //R버튼을 눌렀는데,
    //1. 총을 들고 있고 
    //2. 총에 알맞은 총알이 있다면 
    //3. 갖고있는 총알 개수 내에서 (장탄수-현재 장전된 총알개수) 만큼 총알을 장전해준다
    if (m_currentOnceKey._R)
    {
        if (inven.m_hand)
        {
            TAG_RES_STATIC tag = inven.m_hand->GetTagResStatic(); //총 종류
            TAG_RES_STATIC ammoType = ItemInfo::GetAmmoType(tag); //탄약 종류
            int magSize = static_cast<int>(ItemInfo::GetMagazineSize(tag)); //장탄 수
            int numBulletCurrentLoad = inven.m_hand->GetNumBullet(); //장전되어있는 총알 수

            if (numBulletCurrentLoad == magSize) //이미 가득 장전 되어있는 경우
                return;

            //총에 알맞는 총알이 있는지 확인해서 장전
            auto it = inven.m_mapInventory.find(ammoType);
            if (it != inven.m_mapInventory.end())
            {
                int numBulletInInventory = (*it).second.back()->GetCount(); //인벤토리에 있는 총알 수
                int numBulletNeedReload = magSize - numBulletCurrentLoad;   //장전할 총알 수 (장탄수 - 현재 장전된 총알 개수)

                cout << ItemInfo::GetName(ammoType);
                cout << "을 " << ItemInfo::GetName(tag) << "에 장전" << endl;
                cout << "인벤토리에 있는 총알 수: " << numBulletInInventory << "\n";

                inven.m_numReload = 0;
                if (numBulletInInventory >= numBulletNeedReload)
                {
                    inven.m_hand->SetNumBullet(numBulletNeedReload);
                    (*it).second.back()->SetCount(numBulletInInventory - numBulletNeedReload);

                    inven.m_numReload = numBulletNeedReload;
                }
                else
                {
                    inven.m_hand->SetNumBullet(numBulletInInventory);
                    (*it).second.back()->SetCount(0);

                    inven.m_numReload = numBulletInInventory;
                }
                cout << "장정된 총알 개수: " << inven.m_hand->GetNumBullet() << "\n";
                cout << "인벤토리에 남아있는 총알 개수: " << (*it).second.back()->GetCount() << "\n";

                /*
                TODO: 장전 애니메이션 추가 - Prone인 경우
                */
                if (tag == TAG_RES_STATIC::QBZ)
                {
                    m_pAnimation->Set(
                        CharacterAnimation::BodyPart::UPPER,
                        TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Base,
                        false,
                        [this]()
                    {
                        m_pAnimation->Set(
                            CharacterAnimation::BodyPart::UPPER,
                            TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle,
                            false);
                    });
                }
                else if (tag == TAG_RES_STATIC::Kar98k)
                {
                    if (inven.m_numReload == 5)
                    {
                        // fast reload
                        m_pAnimation->Set(
                            CharacterAnimation::BodyPart::UPPER,
                            TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Base,
                            false, [this]() 
                        {
                            m_pAnimation->Set(
                            CharacterAnimation::BodyPart::UPPER,
                            TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle,
                            false);                       
                        });
                    }
                    else
                    {
                        m_pAnimation->Set(
                            CharacterAnimation::BodyPart::UPPER,
                            TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Base,
                            false,
                            std::bind(&Character::onKar98kReload, this));
                    }
                }
            }
            else
            {
                cout << "총알이 필요해" << endl;
                //do nothing
            }
        }
        else //inven.m_hand == nullptr
        {
            cout << "총을 장착해줘" << endl;
            //do nothing
        }
    }
}

void Character::setRifleOnHand(TAG_RIFLE tagRifle)
{
    //주무기를 손에 든다
    if (tagRifle == TAG_RIFLE::Primary)
    {
        m_pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand,
            false, [this]()
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::UPPER,
                TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle,
                false);
        });
    }
    //보조무기를 손에 든다
    else if (tagRifle == TAG_RIFLE::Secondary)
    {
        m_pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand,
            false, [this]()
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::UPPER,
                TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle,
                false);
        });
    }
}

void Character::setRifleOnBody(TAG_RIFLE tagRifle)
{
    TotalInventory& inven = m_totalInventory;

    //주무기를 다시 몸에 장착
    if (tagRifle == TAG_RIFLE::Primary)
    {
        m_pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static,
            false,
            [this, &inven]()
        {
            inven.m_weaponPrimary = inven.m_hand;
            inven.m_hand = nullptr;
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::UPPER,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1,
                false);
        });
    }
    //보조무기를 다시 몸에 장착
    else if (tagRifle == TAG_RIFLE::Secondary)
    {
        m_pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static,
            false,
            [this, &inven]()
        {
            inven.m_weaponSecondary = inven.m_hand;
            inven.m_hand = nullptr;
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::UPPER,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false);
        });
    }
}

/*
Kar98k 재장전 애니메이션 관련
*/
void Character::onKar98kReloadEnd()
{
    m_pAnimation->Set(
        CharacterAnimation::BodyPart::UPPER,
        TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Base,
        false,
        [this]() 
    {
        m_pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle,
            false);
    });
}

void Character::onKar98kReload()
{
    if (m_totalInventory.m_numReload == 1)
    {
        onKar98kReloadEnd();
    }
    else
    {
        m_pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Base,
            false,
            std::bind(&Character::onKar98kReload, this));
    }

    --m_totalInventory.m_numReload;
}
