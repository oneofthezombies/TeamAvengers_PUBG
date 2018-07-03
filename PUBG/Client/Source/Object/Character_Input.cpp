#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "Item.h"
#include "ItemInfo.h"

void Character::setAttacking()
{
    //for Debug
    TAG_RES_STATIC tag;
    Item* hand = m_totalInventory.m_hand;
    if (hand)
    {
        Debug << "On hand Weapon: ";
        tag = hand->GetTagResStatic();
        Debug << ItemInfo::GetName(tag) << "\n";
    }
    Debug << "Attacking: " << ForDebugGetAttacking(m_attacking) << "\n";

    TotalInventory& inven = m_totalInventory;

    if (m_pAnimation->HasUpperFinishEvent()) return;

    if (m_currentOnceKey._Num1)
    {
        if (inven.m_weaponPrimary)
        {
            //등짝에 주무기가 있다
            if (m_attacking == Attacking::Unarmed)
            {
                m_attacking = Attacking::Rifle;

                inven.m_hand = inven.m_weaponPrimary;
                inven.m_weaponPrimary = nullptr;
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
            else if (m_attacking == Attacking::Rifle)
            {
                //보조무기를 해제하고, 주무기를 장착한다
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::UPPER,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static,
                    false, [this, &inven]()
                {
                    inven.m_weaponSecondary = inven.m_hand;

                    inven.m_hand = inven.m_weaponPrimary;
                    inven.m_weaponPrimary = nullptr;
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
            //등짝에 보조무기가 있다
            if (m_attacking == Attacking::Unarmed)
            {
                m_attacking = Attacking::Rifle;

                inven.m_hand = inven.m_weaponSecondary;
                inven.m_weaponSecondary = nullptr;
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
            else if (m_attacking == Attacking::Rifle)
            {
                //주무기를 해제하고 보조무기를 장착한다
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::UPPER,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static,
                    false, [this, &inven]()
                {
                    inven.m_weaponPrimary = inven.m_hand;

                    inven.m_hand = inven.m_weaponSecondary;
                    inven.m_weaponSecondary = nullptr;
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
                });
            }
            else if (m_attacking == Attacking::Melee)
            {
                //TODO: melee 생기면 구현
            }
        }
    }
    //for test 'X' - 다시 X누르면 이전 무기 장착되는 것도 해야함
    else if (m_currentOnceKey._X)
    {
        //손에 무기를 들고 있다면 해제한다
        //무기가 주무기냐, 보조무기냐에 따라서 다른 애니메이션을 실행한다. 우선 QBZ 주무기 Kar98k 보조무기
        if (inven.m_hand)
        {
            TAG_RES_STATIC tag = inven.m_hand->GetTagResStatic();
            if (tag == TAG_RES_STATIC::QBZ)
            {
                m_attacking = Attacking::Unarmed;
                //주무기를 해제한다
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
                        false); //TODO: 해제는 되는데 다리가 부자연스러움
                });
            }
            else if (tag == TAG_RES_STATIC::Kar98k)
            {
                m_attacking = Attacking::Unarmed;
                //보조무기를 해제한다
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
