#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "Item.h"
#include "ItemInfo.h"

void Character::setAttacking() //Num1, Num2, X
{
    TotalInventory& inven = m_totalInventory;

    //if (pAnimation->HasUpperFinishEvent()) return; //애니메이션 체인 관련 

    if (m_currentOnceKey._Num1)
    {
        cout << "Num1" << endl;
        if (inven.m_pWeaponPrimary)
        {
            if (m_attacking == Attacking::Unarmed)    //주무기를 장착한다
            {
                m_attacking = Attacking::Rifle;

                inven.m_pHand = inven.m_pWeaponPrimary;
                inven.m_pWeaponPrimary = nullptr;

                setRifleOnHand(TAG_RIFLE::Primary);
            }
            else if (m_attacking == Attacking::Rifle) //보조무기를 해제하고, 주무기를 장착한다
            {
                TAG_ANIM_CHARACTER temp = TAG_ANIM_CHARACTER::COUNT;

                if (m_stance == Stance::Stand)
                    temp = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static;
                else if (m_stance == Stance::Prone)
                    temp = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot;

                pAnimation->Set(
                    CharacterAnimation::BodyPart::UPPER,
                    temp,
                    false, 
                    CharacterAnimation::DEFAULT_BLENDING_TIME,
                    CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                    CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                    CharacterAnimation::DEFAULT_POSITION,
                    [this, &inven]()
                {
                    inven.m_pWeaponSecondary = inven.m_pHand;
                    inven.m_pHand = inven.m_pWeaponPrimary;
                    inven.m_pWeaponPrimary = nullptr;

                    setRifleOnHand(TAG_RIFLE::Primary);
                });
            }
            else if (m_attacking == Attacking::Melee)
            {
                //TODO: melee 생기면 구현
            }
        }
    }
    
    if (m_currentOnceKey._Num2)
    {
        cout << "Num2" << endl;
        if (inven.m_pWeaponSecondary)
        {
            if (m_attacking == Attacking::Unarmed)
            {
                m_attacking = Attacking::Rifle;

                inven.m_pHand = inven.m_pWeaponSecondary;
                inven.m_pWeaponSecondary = nullptr;

                setRifleOnHand(TAG_RIFLE::Secondary);
            }
            else if (m_attacking == Attacking::Rifle) //주무기를 등짝에 붙이고 보조무기를 손에 든다
            {
                TAG_ANIM_CHARACTER temp = TAG_ANIM_CHARACTER::COUNT;

                if (m_stance == Stance::Stand)
                    temp = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static;
                else if (m_stance == Stance::Prone)
                    temp = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot;

                pAnimation->Set(
                    CharacterAnimation::BodyPart::UPPER,
                    temp,
                    false, 
                    CharacterAnimation::DEFAULT_BLENDING_TIME,
                    CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                    CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                    CharacterAnimation::DEFAULT_POSITION,
                    [this, &inven]()
                {
                    inven.m_pWeaponPrimary = inven.m_pHand;
                    inven.m_pHand = inven.m_pWeaponSecondary;
                    inven.m_pWeaponSecondary = nullptr;

                    setRifleOnHand(TAG_RIFLE::Secondary);
                });
            }
            else if (m_attacking == Attacking::Melee)
            {
                //TODO: melee 생기면 구현
            }
        }
    }
    
    if (m_currentOnceKey._X)
    {
        cout << "X" << endl;
        //무기가 주무기냐, 보조무기냐에 따라서 다른 애니메이션을 실행한다. 우선 QBZ 주무기 Kar98k 보조무기
        //등에 부착한다
        if (inven.m_pHand)
        {
            m_attacking = Attacking::Unarmed;
            inven.pTempSaveWeaponForX = inven.m_pHand;
            TAG_RES_STATIC tag = inven.m_pHand->GetTagResStatic();

            if (tag == TAG_RES_STATIC::QBZ)
                setRifleOnBody(TAG_RIFLE::Primary);
            else if (tag == TAG_RES_STATIC::Kar98k)
                setRifleOnBody(TAG_RIFLE::Secondary);
        }
        //손에 무기를 들고 있지않는데 X버튼을 누른다면, 이전에 장착했던 무기를 다시 손에 든다
        else
        {
            if (inven.pTempSaveWeaponForX)
            {
                m_attacking = Attacking::Rifle;
                inven.m_pHand = inven.pTempSaveWeaponForX;
                inven.pTempSaveWeaponForX = nullptr;
                TAG_RES_STATIC tag = inven.m_pHand->GetTagResStatic();

                if (tag == TAG_RES_STATIC::QBZ)
                {
                    inven.m_pWeaponPrimary = nullptr;
                    setRifleOnHand(TAG_RIFLE::Primary);
                }
                else if (tag == TAG_RES_STATIC::Kar98k)
                {
                    inven.m_pWeaponSecondary = nullptr;
                    setRifleOnHand(TAG_RIFLE::Secondary);                
                }
            }
        }
    }
}

void Character::setStance() //C, Z
{
    if (m_currentOnceKey._C)
    {
        cout << "C" << endl;
        //서 있으면 쭈그리고, 쭈그리고 있으면 서고, 엎드려 있으면 쭈그린다
        // TODO : check obstacle
        if (m_stance == Stance::Stand)
        {
            m_stance = Stance::Crouch;
            setStandTo(m_stance);
        }
        else if (m_stance == Stance::Crouch)
        {
            m_stance = Stance::Stand;
            setCrouchTo(m_stance);
        }
        else if (m_stance == Stance::Prone)
        {
            m_stance = Stance::Crouch;
            setProneTo(m_stance);
        }
    }
    
    if (m_currentOnceKey._Z)
    {
        cout << "Z" << endl;
        //서 있으면 엎드리고, 쭈그려있으면 엎드리고, 엎드려있으면 선다
        // TODO : check obstacle
        if (m_stance == Stance::Stand)
        {
            m_stance = Stance::Prone;
            setStandTo(m_stance);
        }
        else if (m_stance == Stance::Crouch)
        {
            m_stance = Stance::Prone;
            setCrouchTo(m_stance);
        }
        else if (m_stance == Stance::Prone)
        {
            m_stance = Stance::Stand;
            setProneTo(m_stance);
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
        if (inven.m_pHand)
        {
            TAG_RES_STATIC tag = inven.m_pHand->GetTagResStatic();          //총 종류
            TAG_RES_STATIC ammoType = ItemInfo::GetAmmoType(tag);           //탄약 종류
            int magSize = static_cast<int>(ItemInfo::GetMagazineSize(tag)); //장탄 수
            int numBulletCurrentLoad = inven.m_pHand->GetNumBullet();       //장전되어있는 총알 수

            if (numBulletCurrentLoad == magSize) //이미 가득 장전 되어있는 경우
            {
                cout << "이미 가득차있다!!" << endl;
                return;
            }

            //총에 알맞는 총알이 있는지 확인해서 장전
            auto it = inven.m_mapInventory.find(ammoType);
            if (it != inven.m_mapInventory.end())
            {
                int numBulletInInventory = (*it).second.back()->GetCount(); //인벤토리에 있는 총알 수
                int numBulletNeedReload = magSize - numBulletCurrentLoad;   //장전할 총알 수 (장탄수 - 현재 장전된 총알 개수)

                cout << ItemInfo::GetName(ammoType);
                cout << "을 " << ItemInfo::GetName(tag) << "에 장전" << endl;
                cout << "인벤토리에 있는 총알 수: " << numBulletInInventory << "\n";

                if (numBulletInInventory == 0)
                {
                    cout << "인벤토리에 더이상 총알이 없어 ㅠㅠ" << endl;
                    return;
                }

                inven.m_numReload = 0;
                if (numBulletInInventory >= numBulletNeedReload)
                {
                    int numBullet = inven.m_pHand->GetNumBullet();
                    inven.m_pHand->SetNumBullet(numBullet + numBulletNeedReload);
                    (*it).second.back()->SetCount(numBulletInInventory - numBulletNeedReload);

                    inven.m_numReload = numBulletNeedReload;
                }
                else
                {
                    inven.m_pHand->SetNumBullet(numBulletInInventory);
                    (*it).second.back()->SetCount(0);

                    inven.m_numReload = numBulletInInventory;
                }
                cout << "장전한 총알 개수: " << inven.m_numReload << "\n";
                cout << "총에 장전된 총알 개수: " << inven.m_pHand->GetNumBullet() << "\n";
                cout << "인벤토리에 남아있는 총알 개수: " << (*it).second.back()->GetCount() << "\n";

                /*
                TODO: 장전 애니메이션 추가 - Prone인 경우
                */
                if (tag == TAG_RES_STATIC::QBZ)
                {
                    pAnimation->Set(
                        CharacterAnimation::BodyPart::UPPER,
                        TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Base,
                        false,
                        CharacterAnimation::DEFAULT_BLENDING_TIME,
                        CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                        CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                        CharacterAnimation::DEFAULT_POSITION,
                        [this]()
                    {
                        pAnimation->Set(
                            CharacterAnimation::BodyPart::BOTH,
                            m_lowerAnimState,
                            false);
                    });
                }
                else if (tag == TAG_RES_STATIC::Kar98k)
                {
                    if (inven.m_numReload == 5)
                    {
                        // fast reload
                        pAnimation->Set(
                            CharacterAnimation::BodyPart::UPPER,
                            TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Base,
                            false,
                            CharacterAnimation::DEFAULT_BLENDING_TIME,
                            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                            CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                            CharacterAnimation::DEFAULT_POSITION,
                            [this]() 
                        {
                            pAnimation->Set(
                            CharacterAnimation::BodyPart::BOTH,
                            m_lowerAnimState,
                            false);                       
                        });
                    }
                    else
                    {
                        pAnimation->Set(
                            CharacterAnimation::BodyPart::UPPER,
                            TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Base,
                            false,
                            CharacterAnimation::DEFAULT_BLENDING_TIME,
                            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                            CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                            CharacterAnimation::DEFAULT_POSITION,
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
        else //inven.m_pHand == nullptr
        {
            cout << "총을 장착해줘" << endl;
            //do nothing
        }
    }
}

/*
무기 장착 및 해제 애니메이션
*/
void Character::setRifleOnHand(TAG_RIFLE tagRifle)
{  
    //애니메이션 정하기
    TAG_ANIM_CHARACTER animTag = TAG_ANIM_CHARACTER::COUNT;
    if (tagRifle == TAG_RIFLE::Primary)        //주무기 일 때
    {
        if (m_stance == Stance::Stand)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot_OnHand;
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //보조무기 일 때
    {
        if (m_stance == Stance::Stand)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot_OnHand;
    }
    assert((animTag != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnHand(), animTag is COUNT");

    //애니메이션 적용
    pAnimation->Set(
        CharacterAnimation::BodyPart::UPPER,
        animTag,
        false, 
        CharacterAnimation::DEFAULT_BLENDING_TIME,
        CharacterAnimation::DEFAULT_NEXT_WEIGHT,
        CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
        CharacterAnimation::DEFAULT_POSITION,
        [this]()
    {
        pAnimation->Set(
            CharacterAnimation::BodyPart::BOTH,
            m_lowerAnimState,
            false);
    });
}

void Character::setRifleOnBody(TAG_RIFLE tagRifle)
{
    TotalInventory& inven = m_totalInventory;

    if (tagRifle == TAG_RIFLE::Primary) //주무기를 다시 몸에 장착
    {
        //애니메이션 정하기
        TAG_ANIM_CHARACTER animTag = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static;
        else if (m_stance == Stance::Prone)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot;    

        assert((animTag != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnBody(), animTag is COUNT");

        //애니메이션 적용
        pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            animTag,
            false,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
            CharacterAnimation::DEFAULT_POSITION,
            [this, &inven]()
        {
            inven.m_pWeaponPrimary = inven.m_pHand;
            inven.m_pHand = nullptr;
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                m_lowerAnimState,
                false);
        });      
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //보조무기를 다시 몸에 장착
    {
        //애니메이션 정하기
        TAG_ANIM_CHARACTER animTag = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static;
        else if (m_stance == Stance::Prone)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot;

        assert((animTag != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnBody(), animTag is COUNT");

        //애니메이션 적용
        pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            animTag,
            false,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
            CharacterAnimation::DEFAULT_POSITION,
            [this, &inven]()
        {
            inven.m_pWeaponSecondary = inven.m_pHand;
            inven.m_pHand = nullptr;
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                m_lowerAnimState,
                false);
        });
    }
}

/*
자세 전이 관련
*/
/*

서있다가 앉음 주기 2.8
앉아있다가 섬 주기 2.9

서있다가 앉다가 섬 -> 앉아있다가 섬의 위치 = 앉아있다가 섬 주기(또는 서있다가 앉음) - 서있다가 앉음의 위치 
*/
void Character::setStandTo(Stance stance)
{
    if (m_attacking == Attacking::Unarmed)
    {
        if (stance == Stance::Crouch)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Crouch,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                1.0f,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState, 
                    true,
                    1.0f,
                    CharacterAnimation::DEFAULT_NEXT_WEIGHT);
            });
        }
        else if (stance == Stance::Prone)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Prone,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });       
        }
    }
    else if (m_attacking == Attacking::Rifle)
    {
        if (stance == Stance::Crouch)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_Crouch,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        }
        else if (stance == Stance::Prone)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_Prone,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        }
    
    }
}

void Character::setCrouchTo(Stance stance)
{
    if (m_attacking == Attacking::Unarmed)
    {
        if (stance == Stance::Stand)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Stand,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        }
        else if (stance == Stance::Prone)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Prone,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });       
        }
    }
    else if (m_attacking == Attacking::Rifle)
    {
        if (stance == Stance::Stand)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Base_Stand,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        }
        else if (stance == Stance::Prone)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Base_Prone,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        }
    }
}

void Character::setProneTo(Stance stance)
{
    if (m_attacking == Attacking::Unarmed)
    {
        if (stance == Stance::Stand)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Stand,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        }
        else if (stance == Stance::Crouch)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Crouch,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        }
    }
    else if (m_attacking == Attacking::Rifle)
    {
        if (stance == Stance::Stand)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base_Stand,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        }
        else if (stance == Stance::Crouch)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base_Crouch,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    false);
            });
        } 
    }
}

/*
Kar98k 재장전 애니메이션 관련
*/
void Character::onKar98kReloadEnd()
{
    pAnimation->Set(
        CharacterAnimation::BodyPart::UPPER,
        TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Base,
        false,
        CharacterAnimation::DEFAULT_BLENDING_TIME,
        CharacterAnimation::DEFAULT_NEXT_WEIGHT,
        CharacterAnimation::DEFAULT_POSITION,
        CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
        [this]() 
    {
        pAnimation->Set(
            CharacterAnimation::BodyPart::BOTH,
            m_lowerAnimState,
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
        pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Base,
            false,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
            std::bind(&Character::onKar98kReload, this));
    }

    --m_totalInventory.m_numReload;
}
