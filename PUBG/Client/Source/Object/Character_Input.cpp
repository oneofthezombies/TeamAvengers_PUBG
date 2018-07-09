#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "Item.h"
#include "ItemInfo.h"

void Character::setAttacking() //Num1, Num2, X
{
    TotalInventory& inven = m_totalInventory;

    //if (pAnimation->HasUpperFinishEvent()) return; //�ִϸ��̼� ü�� ���� 

    if (m_currentOnceKey._Num1)
    {
        cout << "Num1" << endl;
        if (inven.m_pWeaponPrimary)
        {
            if (m_attacking == Attacking::Unarmed)    //�ֹ��⸦ �����Ѵ�
            {
                m_attacking = Attacking::Rifle;

                inven.m_pHand = inven.m_pWeaponPrimary;
                inven.m_pWeaponPrimary = nullptr;

                setRifleOnHand(TAG_RIFLE::Primary);
            }
            else if (m_attacking == Attacking::Rifle) //�������⸦ �����ϰ�, �ֹ��⸦ �����Ѵ�
            {
                TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;

                if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static;
                else if (m_stance == Stance::Prone)
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot;
                
                assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::setAttacking(), tagAnim is COUNT");

                m_hasChangingState = true;
                pAnimation->Set(
                    CharacterAnimation::BodyPart::UPPER,
                    tagAnim,
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
                //TODO: melee ����� ����
            }
        }
    }
    
    else if (m_currentOnceKey._Num2)
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
            else if (m_attacking == Attacking::Rifle) //�ֹ��⸦ ��¦�� ���̰� �������⸦ �տ� ���
            {
                TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;

                if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static;
                else if (m_stance == Stance::Prone)
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot;

                m_hasChangingState = true;
                pAnimation->Set(
                    CharacterAnimation::BodyPart::UPPER,
                    tagAnim,
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
                //TODO: melee ����� ����
            }
        }
    }
    
    else if (m_currentOnceKey._X)
    {
        cout << "X" << endl;
        //���Ⱑ �ֹ����, ��������Ŀ� ���� �ٸ� �ִϸ��̼��� �����Ѵ�. �켱 QBZ �ֹ��� Kar98k ��������
        //� �����Ѵ�
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
        //�տ� ���⸦ ��� �����ʴµ� X��ư�� �����ٸ�, ������ �����ߴ� ���⸦ �ٽ� �տ� ���
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
        //�� ������ �ޱ׸���, �ޱ׸��� ������ ����, ����� ������ �ޱ׸���
        // TODO : check obstacle
        if (m_stance == Stance::Stand)
        {
            m_stance = Stance::Crouch;
            setStandTo();
        }
        else if (m_stance == Stance::Crouch)
        {
            m_stance = Stance::Stand;
            setCrouchTo();
        }
        else if (m_stance == Stance::Prone)
        {
            m_stance = Stance::Crouch;
            setProneTo();
        }
    }
    
    else if (m_currentOnceKey._Z)
    {
        cout << "Z" << endl;
        //�� ������ ���帮��, �ޱ׷������� ���帮��, ����������� ����
        // TODO : check obstacle
        if (m_stance == Stance::Stand)
        {
            m_stance = Stance::Prone;
            setStandTo();
        }
        else if (m_stance == Stance::Crouch)
        {
            m_stance = Stance::Prone;
            setCrouchTo();
        }
        else if (m_stance == Stance::Prone)
        {
            m_stance = Stance::Stand;
            setProneTo();
        }
    }
}

void Character::setReload()
{
    TotalInventory& inven = m_totalInventory;

    //R��ư�� �����µ�,
    //1. ���� ��� �ְ� 
    //2. �ѿ� �˸��� �Ѿ��� �ִٸ� 
    //3. �����ִ� �Ѿ� ���� ������ (��ź��-���� ������ �Ѿ˰���) ��ŭ �Ѿ��� �������ش�
    if (m_currentOnceKey._R)
    {
        if (inven.m_pHand)
        {
            TAG_RES_STATIC tag = inven.m_pHand->GetTagResStatic();          //�� ����
            TAG_RES_STATIC ammoType = ItemInfo::GetAmmoType(tag);           //ź�� ����
            int magSize = static_cast<int>(ItemInfo::GetMagazineSize(tag)); //��ź ��
            int numBulletCurrentLoad = inven.m_pHand->GetNumBullet();       //�����Ǿ��ִ� �Ѿ� ��

            if (numBulletCurrentLoad == magSize) //�̹� ���� ���� �Ǿ��ִ� ���
            {
                cout << "�̹� �������ִ�!!" << endl;
                return;
            }

            //�ѿ� �˸´� �Ѿ��� �ִ��� Ȯ���ؼ� ����
            auto it = inven.m_mapInventory.find(ammoType);
            if (it != inven.m_mapInventory.end())
            {
                int numBulletInInventory = (*it).second.back()->GetCount(); //�κ��丮�� �ִ� �Ѿ� ��
                int numBulletNeedReload = magSize - numBulletCurrentLoad;   //������ �Ѿ� �� (��ź�� - ���� ������ �Ѿ� ����)

                cout << ItemInfo::GetName(ammoType);
                cout << "�� " << ItemInfo::GetName(tag) << "�� ����" << endl;
                cout << "�κ��丮�� �ִ� �Ѿ� ��: " << numBulletInInventory << "\n";

                if (numBulletInInventory == 0)
                {
                    cout << "�κ��丮�� ���̻� �Ѿ��� ���� �Ф�" << endl;
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
                cout << "������ �Ѿ� ����: " << inven.m_numReload << "\n";
                cout << "�ѿ� ������ �Ѿ� ����: " << inven.m_pHand->GetNumBullet() << "\n";
                cout << "�κ��丮�� �����ִ� �Ѿ� ����: " << (*it).second.back()->GetCount() << "\n";

                if (tag == TAG_RES_STATIC::QBZ)
                {
                    if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
                    {
                        m_hasChangingState = true;
                        pAnimation->Set(
                            CharacterAnimation::BodyPart::UPPER,
                            TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Base,
                            true, //ok
                            CharacterAnimation::DEFAULT_BLENDING_TIME,
                            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                            CharacterAnimation::DEFAULT_POSITION,
                            0.3f, //ok
                            [this]()
                        {
                            m_hasChangingState = false;
                            pAnimation->Set(
                                CharacterAnimation::BodyPart::BOTH,
                                m_lowerAnimState,
                                true,
                                0.3f);
                        });


                    }
                    else if (m_stance == Stance::Prone)
                    {
                        m_hasChangingState = true;
                        pAnimation->Set(
                            CharacterAnimation::BodyPart::UPPER,
                            TAG_ANIM_CHARACTER::Weapon_QBZ_Reload_Prone,
                            true, //ok
                            CharacterAnimation::DEFAULT_BLENDING_TIME,
                            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                            CharacterAnimation::DEFAULT_POSITION,
                            0.3f, //ok
                            [this]()
                        {
                            m_hasChangingState = false;
                            pAnimation->Set(
                                CharacterAnimation::BodyPart::BOTH,
                                m_lowerAnimState,
                                true,
                                0.3f);
                        });                    
                    }
                }
                else if (tag == TAG_RES_STATIC::Kar98k)
                {
                    if (inven.m_numReload == 5)
                    {
                        //�� ��ü �ִϸ��̼�
                        m_isNeedRifleAnim = true;
                        inven.m_pHand->Set
                        (
                            TAG_ANIM_WEAPON::Weapon_Kar98k_Reload_Fast,
                            false,
                            Item::DEFAULT_BLENDING_TIME,
                            Item::DEFAULT_NEXT_WEIGHT,
                            Item::DEFAULT_POSITION,
                            Item::DEFAULT_FINISH_EVENT_AGO_TIME,
                            [this, &inven]() {
                            inven.m_pHand->Set(
                                TAG_ANIM_WEAPON::Weapon_Kar98k_Idle,
                                false);
                            m_isNeedRifleAnim = false;
                        });

                        // fast reload
                        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
                        {
                            m_hasChangingState = true;

                            //ĳ���� �ִϸ��̼�
                            pAnimation->Set(
                                CharacterAnimation::BodyPart::UPPER,
                                TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Base,
                                true,
                                CharacterAnimation::DEFAULT_BLENDING_TIME,
                                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                                CharacterAnimation::DEFAULT_POSITION,
                                0.3f,
                                [this, &inven]()
                            {
                                m_hasChangingState = false;
                                pAnimation->Set(
                                    CharacterAnimation::BodyPart::BOTH,
                                    m_lowerAnimState,
                                    true,
                                    0.3f);
                            });
                        }
                        else if (m_stance == Stance::Prone)
                        {
                            m_hasChangingState = true;
                           
                            //ĳ���� �ִϸ��̼�
                            pAnimation->Set(
                                CharacterAnimation::BodyPart::UPPER,
                                TAG_ANIM_CHARACTER::Weapon_Kar98k_ReloadFast_Prone,
                                true,
                                CharacterAnimation::DEFAULT_BLENDING_TIME,
                                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                                CharacterAnimation::DEFAULT_POSITION,
                                0.3f,
                                [this]()
                            {
                                m_hasChangingState = false;
                                pAnimation->Set(
                                    CharacterAnimation::BodyPart::BOTH,
                                    m_lowerAnimState,
                                    true, 
                                    0.3f);
                            });
                        }
                    }
                    else
                    {
                        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
                        {
                            m_hasChangingState = true;                   
                            pAnimation->Set(
                                CharacterAnimation::BodyPart::UPPER,
                                TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Base,
                                true,
                                CharacterAnimation::DEFAULT_BLENDING_TIME,
                                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                                CharacterAnimation::DEFAULT_POSITION,
                                CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
                                std::bind(&Character::onKar98kReload, this));
                        }
                        else if (m_stance == Stance::Prone)
                        {
                            m_hasChangingState = true;
                            pAnimation->Set(
                                CharacterAnimation::BodyPart::UPPER,
                                TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Start_Prone,
                                true, //ok
                                CharacterAnimation::DEFAULT_BLENDING_TIME,
                                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                                CharacterAnimation::DEFAULT_POSITION,
                                0.3f, //ok
                                std::bind(&Character::onKar98kReload, this));                       
                        }
                    }
                }
            }
            else
            {
                cout << "�Ѿ��� �ʿ���" << endl;
                //do nothing
            }
        }
        else //inven.m_pHand == nullptr
        {
            cout << "���� ��������" << endl;
            //do nothing
        }
    }
}

void Character::setPunch()
{
    if (m_attacking == Attacking::Unarmed && m_currentOnceKey._LButton)         //�ָ��� �ֵθ���
    {
        TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
        int randomNum = rand() % 3;
        //�ִϸ��̼� ���ϱ�
        if (m_stance == Stance::Stand)
        {
            switch (randomNum)
            {
            case 0:
                tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_1;
                break;
            case 1:
                tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_2;
                break;
            case 2:
                tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Upperbody_Attack_3;
                break;
            }
        }
        else if (m_stance == Stance::Crouch)
        {
            switch (randomNum)
            {
            case 0:
                tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_1;
                break;
            case 1:
                tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_2;
                break;
            case 2:
                tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Attack_3;
                break;
            }
        }
        else if (m_stance == Stance::Prone)
            return;
        assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && " Character::setPunch(), tagAnim is COUNT");

        //�ִϸ��̼� ����
        pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            tagAnim,
            true,
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

void Character::setInteraction()
{
    //FŰ�� ������ ������, �������ݱ�
    if (m_currentOnceKey._F)
    {
        //TODO: TAG_OBJECT�� ���� �����⳪, ������ �ݱ� �ִϸ��̼��� ����Ǿ���Ѵ�
        //TODO: �������� ���缭����
        bool isDoor = false;
        if (isDoor)
        {
            //������
            TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
            if (m_attacking == Attacking::Unarmed)
            {
                if (m_stance == Stance::Stand)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_DoorOpen;
                }
                else if (m_stance == Stance::Crouch)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_DoorOpen;
                }
                else if (m_stance == Stance::Prone)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_DoorOpen;
                }
            }
            else if (m_attacking == Attacking::Rifle)
            {
                if (m_stance == Stance::Stand)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_DoorOpen;
                }
                else if (m_stance == Stance::Crouch)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_DoorOpen;
                }
                else if (m_stance == Stance::Prone)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_DoorOpen;
                }
            }
            assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::setInteraction(), Door open tagAnim is COUNT");

            //�ִϸ��̼� ����
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                tagAnim,
                true,
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
        else
        {
            //������ �ݱ�
            TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
            if (m_attacking == Attacking::Unarmed)
            {
                if (m_stance == Stance::Stand)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Pickup_Low;
                }
                else if (m_stance == Stance::Crouch)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Pickup_Low;
                }
                else if (m_stance == Stance::Prone)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Pickup_Low;
                }
            }
            else if (m_attacking == Attacking::Rifle)
            {
                if (m_stance == Stance::Stand)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Pickup_Low;
                }
                else if (m_stance == Stance::Crouch)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Pickup_Low;
                }
                else if (m_stance == Stance::Prone)
                {
                    tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Pickup_Low;
                }
            }
            assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::setInteraction(), Pick up tagAnim is COUNT");

            //�ִϸ��̼� ����
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                tagAnim,
                true,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                0.3f,
                0.3f,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    true,
                    0.3f,
                    CharacterAnimation::DEFAULT_NEXT_WEIGHT);
            });
        }
    }
}

void Character::setJump()
{
    if (m_currentOnceKey._Space && m_currentStayKey._W)
    {
        TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
        if (m_attacking == Attacking::Unarmed)
            tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_F;
        else if (m_attacking == Attacking::Rifle)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Jump_F;

        pAnimation->Set(
            CharacterAnimation::BodyPart::BOTH,
            tagAnim,
            true,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            0.3f,
            [this]()
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                m_lowerAnimState,
                true,
                0.3f,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT);
        });
    }
    else if (m_currentOnceKey._Space)
    {
        TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
        if(m_attacking == Attacking::Unarmed)
            tagAnim = TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_Stationary;
        else if(m_attacking == Attacking::Rifle)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Jump_Stationary_Full_001;
    
        //�ִϸ��̼� ����
        pAnimation->Set(
            CharacterAnimation::BodyPart::BOTH,
            tagAnim,
            true,
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

/*
���� ���� �� ���� �ִϸ��̼�
*/
void Character::setRifleOnHand(TAG_RIFLE tagRifle)
{  
    //�ִϸ��̼� ���ϱ�
    TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
    if (tagRifle == TAG_RIFLE::Primary)        //�ֹ��� �� ��
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot_OnHand;
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //�������� �� ��
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot_OnHand;
    }
    assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnHand(), tagAnim is COUNT");

    //�ִϸ��̼� ����
    pAnimation->Set(
        CharacterAnimation::BodyPart::UPPER,
        tagAnim,
        false, //�տ� �鶧 �����ϸ� ���� �鸮�� ������ ���� false
        CharacterAnimation::DEFAULT_BLENDING_TIME,
        CharacterAnimation::DEFAULT_NEXT_WEIGHT,
        CharacterAnimation::DEFAULT_POSITION,
        0.3f,
        [this]()
    {
        m_hasChangingState = false;

        pAnimation->Set(
            CharacterAnimation::BodyPart::BOTH,
            m_lowerAnimState,
            true,
            0.3f,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT);
    });
}

void Character::setRifleOnBody(TAG_RIFLE tagRifle)
{
    /*
    - ���� �����ϴ� �ִϸ��̼� - Idling �ִϸ��̼� 0.3f ����, Prone���¿��� �����Ÿ��� ���� �ذ�
    */

    TotalInventory& inven = m_totalInventory;

    if (tagRifle == TAG_RIFLE::Primary) //�ֹ��⸦ �ٽ� ���� ����
    {
        //�ִϸ��̼� ���ϱ�
        TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot;

        assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnBody(), tagAnim is COUNT");

        //�ִϸ��̼� ����
        m_hasChangingState = true;
        pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            tagAnim,
            false,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            0.3f,
            [this, &inven]()
        {
            inven.m_pWeaponPrimary = inven.m_pHand;
            inven.m_pHand = nullptr;
            m_hasChangingState = false;

            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                m_lowerAnimState,
                true,
                0.3f,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT);
        });
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //�������⸦ �ٽ� ���� ����
    {
        //�ִϸ��̼� ���ϱ�
        TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot;

        assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnBody(), tagAnim is COUNT");

        //�ִϸ��̼� ����
        m_hasChangingState = true;
        pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            tagAnim,
            false,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            0.3f,
            [this, &inven]()
        {
            inven.m_pWeaponSecondary = inven.m_pHand;
            inven.m_pHand = nullptr;
            m_hasChangingState = false;

            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                m_lowerAnimState,
                true,
                0.3f,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT);
        });
    }
}

/*
�ڼ� ���� ����
*/
/*

���ִٰ� ���� �ֱ� 2.8
�ɾ��ִٰ� �� �ֱ� 2.9

���ִٰ� �ɴٰ� �� -> �ɾ��ִٰ� ���� ��ġ = �ɾ��ִٰ� �� �ֱ�(�Ǵ� ���ִٰ� ����) - ���ִٰ� ������ ��ġ 
*/
void Character::setStandTo()
{
    if (m_attacking == Attacking::Unarmed)
    {
        if (m_stance == Stance::Crouch)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Crouch,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                0.4f,
                1.2f,
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState, 
                    true,
                    1.2f,
                    CharacterAnimation::DEFAULT_NEXT_WEIGHT);
            });
        }
        else if (m_stance == Stance::Prone)
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
        if (m_stance == Stance::Crouch)
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
        else if (m_stance == Stance::Prone)
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

void Character::setCrouchTo()
{
    if (m_attacking == Attacking::Unarmed)
    {
        if (m_stance == Stance::Stand)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Stand,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                0.6f,
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
        else if (m_stance == Stance::Prone)
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
        if (m_stance == Stance::Stand)
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
        else if (m_stance == Stance::Prone)
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

void Character::setProneTo()
{
    if (m_attacking == Attacking::Unarmed)
    {
        if (m_stance == Stance::Stand)
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
        else if (m_stance == Stance::Crouch)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Crouch,
                false,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                0.5f, //�̰� ����� ���� �Ÿ��� ����
                [this]()
            {
                pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    m_lowerAnimState,
                    true,
                    0.5f,
                    CharacterAnimation::DEFAULT_NEXT_WEIGHT);
            });
        }
    }
    else if (m_attacking == Attacking::Rifle)
    {
        if (m_stance == Stance::Stand)
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
        else if (m_stance == Stance::Crouch)
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
Kar98k ������ �ִϸ��̼� ����
*/
void Character::onKar98kReloadEnd()
{
    //ĳ������ �ִϸ��̼�
    if (m_stance == Stance::Stand)
    {
        pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Base,
            false, //ok
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            0.3f, //ok
            [this]()
        {
            m_hasChangingState = false;
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                m_lowerAnimState,
                true,
                0.3f);
        });
    }
    else if (m_stance == Stance::Prone)
    {
        pAnimation->Set(
            CharacterAnimation::BodyPart::UPPER,
            TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_End_Prone,
            false, //ok
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME, //ok
            [this]()
        {
            m_hasChangingState = false;
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                m_lowerAnimState,
                false);
        });  
    }
}

void Character::onKar98kReload()
{
    if (m_totalInventory.m_numReload == 1)
    {
        onKar98kReloadEnd();
    }
    else
    {
        //ĳ������ �ִϸ��̼�
        if (m_stance == Stance::Stand)
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
        else if (m_stance == Stance::Prone)
        {
            pAnimation->Set(
                CharacterAnimation::BodyPart::UPPER,
                TAG_ANIM_CHARACTER::Weapon_Kar98k_Reload_Loop_Prone,
                true, //ok
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_NEXT_WEIGHT,
                CharacterAnimation::DEFAULT_POSITION,
                0.3f, //ok
                std::bind(&Character::onKar98kReload, this));        
        }
    }

    --m_totalInventory.m_numReload;
}