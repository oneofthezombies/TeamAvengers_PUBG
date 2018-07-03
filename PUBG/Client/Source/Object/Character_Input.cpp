#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "Item.h"
#include "ItemInfo.h"

void Character::setAttacking() //Num1, Num2, X
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
            else if (m_attacking == Attacking::Rifle) //�������⸦ �����ϰ�, �ֹ��⸦ �����Ѵ�
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
                //TODO: melee ����� ����
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
            else if (m_attacking == Attacking::Rifle) //�ֹ��⸦ ��¦�� ���̰� �������⸦ �տ� ���
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
                //TODO: melee ����� ����
            }
        }
    }
    else if (m_currentOnceKey._X)
    {
        //���Ⱑ �ֹ����, ��������Ŀ� ���� �ٸ� �ִϸ��̼��� �����Ѵ�. �켱 QBZ �ֹ��� Kar98k ��������
        //� �����Ѵ�
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
        //�տ� ���⸦ ��� �����ʴµ� X��ư�� �����ٸ�, ������ �����ߴ� ���⸦ �ٽ� �տ� ���
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

void Character::setStance() //C, Z
{
    //for Debug =========================================================
    Debug << "Stance: " << ForDebugGetStance(m_stance) << "\n";
    //===================================================================

    if (m_currentOnceKey._C)
    {
        //�� ������ �ޱ׸���, �ޱ׸��� ������ ����, ����� ������ �ޱ׸���
        //TODO: �ΰ��� �ٽ� Ȯ��
        // TODO : check obstacle
        if (m_stance == Stance::Stand)
        {
            cout << "Stand to Crouch" << endl;
            m_stance = Stance::Crouch;
            setStandAnimation(m_stance);
        }
        else if (m_stance == Stance::Crouch)
        {
            cout << "Crouch to Stand" << endl;
            m_stance = Stance::Stand;
            setCrouchAnimation(m_stance);
        }
        else if (m_stance == Stance::Prone)
        {
            cout << "Prone to Crouch" << endl;
            m_stance = Stance::Crouch;
            setProneAnimation(m_stance);
        }
    }
    else if (m_currentOnceKey._Z)
    {
        //�� ������ ���帮��, �ޱ׷������� ���帮��, ����������� ����
        //TODO: �ΰ��� �ٽ� Ȯ��
        // TODO : check obstacle
        if (m_stance == Stance::Stand)
        {
            cout << "Stand to Prone" << endl;
            m_stance = Stance::Prone;
            setStandAnimation(m_stance);
        }
        else if (m_stance == Stance::Crouch)
        {
            cout << "Crouch to Prone" << endl;
            m_stance = Stance::Prone;
            setCrouchAnimation(m_stance);
        }
        else if (m_stance == Stance::Prone)
        {
            cout << "Prone to Stand" << endl;
            m_stance = Stance::Stand;
            setProneAnimation(m_stance);
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
        if (inven.m_hand)
        {
            TAG_RES_STATIC tag = inven.m_hand->GetTagResStatic(); //�� ����
            TAG_RES_STATIC ammoType = ItemInfo::GetAmmoType(tag); //ź�� ����
            int magSize = static_cast<int>(ItemInfo::GetMagazineSize(tag)); //��ź ��
            int numBulletCurrentLoad = inven.m_hand->GetNumBullet(); //�����Ǿ��ִ� �Ѿ� ��

            if (numBulletCurrentLoad == magSize) //�̹� ���� ���� �Ǿ��ִ� ���
                return;

            //�ѿ� �˸´� �Ѿ��� �ִ��� Ȯ���ؼ� ����
            auto it = inven.m_mapInventory.find(ammoType);
            if (it != inven.m_mapInventory.end())
            {
                int numBulletInInventory = (*it).second.back()->GetCount(); //�κ��丮�� �ִ� �Ѿ� ��
                int numBulletNeedReload = magSize - numBulletCurrentLoad;   //������ �Ѿ� �� (��ź�� - ���� ������ �Ѿ� ����)

                cout << ItemInfo::GetName(ammoType);
                cout << "�� " << ItemInfo::GetName(tag) << "�� ����" << endl;
                cout << "�κ��丮�� �ִ� �Ѿ� ��: " << numBulletInInventory << "\n";

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
                cout << "������ �Ѿ� ����: " << inven.m_hand->GetNumBullet() << "\n";
                cout << "�κ��丮�� �����ִ� �Ѿ� ����: " << (*it).second.back()->GetCount() << "\n";

                /*
                TODO: ���� �ִϸ��̼� �߰� - Prone�� ���
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
                cout << "�Ѿ��� �ʿ���" << endl;
                //do nothing
            }
        }
        else //inven.m_hand == nullptr
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
    //�ֹ��⸦ �տ� ���
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
    //�������⸦ �տ� ���
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

    //�ֹ��⸦ �ٽ� ���� ����
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
    //�������⸦ �ٽ� ���� ����
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
Stance ���� �ִϸ��̼�
*/
void Character::setStandAnimation(Stance toStance)
{
    if (m_attacking == Attacking::Unarmed)
    {
        //Stand to Crouch
        if (toStance == Stance::Crouch)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Crouch,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Idling_1, false);
            });
        }
        //Stand to Prone
        else if (toStance == Stance::Prone)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Prone,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Idling_1, false);
            });        
        }
    }
    else if (m_attacking == Attacking::Rifle)
    {
        //Stand to Crouch
        if (toStance == Stance::Crouch)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_Crouch,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Low_Idle_Still, false);
            });
        }
        //Stand to Prone
        else if (toStance == Stance::Prone)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_Prone,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Low_Idle_Still, false);
            });
        }
    
    }
}

void Character::setCrouchAnimation(Stance toStance)
{
    if (m_attacking == Attacking::Unarmed)
    {
        //Crouch to Stand
        if (toStance == Stance::Stand)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Stand,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false);
            });
        }
        //Crouch to Prone
        else if (toStance == Stance::Prone)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Prone,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Idling_1, false);
            });      
        }
    }
    else if (m_attacking == Attacking::Rifle)
    {
        //Crouch to Stand
        if (toStance == Stance::Stand)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Base_Stand,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Low_Idle_Still_Very, false);
            });
        }
        //Crouch to Prone
        else if (toStance == Stance::Prone)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Base_Prone,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Low_Idle_Still, false);
            });
        }

    }
}

void Character::setProneAnimation(Stance toStance)
{
    if (m_attacking == Attacking::Unarmed)
    {
        //Prone to Stand
        if (toStance == Stance::Stand)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Stand,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false);
            });
        }
        //Prone to Crouch
        else if (toStance == Stance::Crouch)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Unarmed_Combat_Prone_Crouch,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Unarmed_Combat_Crouch_Idling_1, false);
            });       
        }
    }
    else if (m_attacking == Attacking::Rifle)
    {
        //Prone to Stand
        if (toStance == Stance::Stand)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base_Stand,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Low_Idle_Still_Very, false);
            });
        }
        //Prone to Crouch
        else if (toStance == Stance::Crouch)
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::Rifle_Combat_Prone_Base_Crouch,
                false,
                [this]()
            {
                m_pAnimation->Set(
                    CharacterAnimation::BodyPart::BOTH,
                    TAG_ANIM_CHARACTER::Rifle_Combat_Crouch_Low_Idle_Still, false);
            });
        }

    }
}


/*
Kar98k ������ �ִϸ��̼� ����
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
