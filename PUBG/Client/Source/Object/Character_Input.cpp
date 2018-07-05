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
                //TODO: melee ����� ����
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
            else if (m_attacking == Attacking::Rifle) //�ֹ��⸦ ��¦�� ���̰� �������⸦ �տ� ���
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
                //TODO: melee ����� ����
            }
        }
    }
    
    if (m_currentOnceKey._X)
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
        //�� ������ ���帮��, �ޱ׷������� ���帮��, ����������� ����
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

                /*
                TODO: ���� �ִϸ��̼� �߰� - Prone�� ���
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

/*
���� ���� �� ���� �ִϸ��̼�
*/
void Character::setRifleOnHand(TAG_RIFLE tagRifle)
{  
    //�ִϸ��̼� ���ϱ�
    TAG_ANIM_CHARACTER animTag = TAG_ANIM_CHARACTER::COUNT;
    if (tagRifle == TAG_RIFLE::Primary)        //�ֹ��� �� ��
    {
        if (m_stance == Stance::Stand)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot_OnHand;
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //�������� �� ��
    {
        if (m_stance == Stance::Stand)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot_OnHand;
    }
    assert((animTag != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnHand(), animTag is COUNT");

    //�ִϸ��̼� ����
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

    if (tagRifle == TAG_RIFLE::Primary) //�ֹ��⸦ �ٽ� ���� ����
    {
        //�ִϸ��̼� ���ϱ�
        TAG_ANIM_CHARACTER animTag = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_Static;
        else if (m_stance == Stance::Prone)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot;    

        assert((animTag != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnBody(), animTag is COUNT");

        //�ִϸ��̼� ����
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
    else if (tagRifle == TAG_RIFLE::Secondary) //�������⸦ �ٽ� ���� ����
    {
        //�ִϸ��̼� ���ϱ�
        TAG_ANIM_CHARACTER animTag = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_Static;
        else if (m_stance == Stance::Prone)
            animTag = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot;

        assert((animTag != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnBody(), animTag is COUNT");

        //�ִϸ��̼� ����
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
�ڼ� ���� ����
*/
/*

���ִٰ� ���� �ֱ� 2.8
�ɾ��ִٰ� �� �ֱ� 2.9

���ִٰ� �ɴٰ� �� -> �ɾ��ִٰ� ���� ��ġ = �ɾ��ִٰ� �� �ֱ�(�Ǵ� ���ִٰ� ����) - ���ִٰ� ������ ��ġ 
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
Kar98k ������ �ִϸ��̼� ����
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
