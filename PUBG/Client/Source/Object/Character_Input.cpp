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
            //��¦�� �ֹ��Ⱑ �ִ�
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
                //�������⸦ �����ϰ�, �ֹ��⸦ �����Ѵ�
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
                //TODO: melee ����� ����
            }
        }
    }
    else if (m_currentOnceKey._Num2)
    {
        if (inven.m_weaponSecondary)
        {
            //��¦�� �������Ⱑ �ִ�
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
                //�ֹ��⸦ �����ϰ� �������⸦ �����Ѵ�
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
                //TODO: melee ����� ����
            }
        }
    }
    //for test 'X' - �ٽ� X������ ���� ���� �����Ǵ� �͵� �ؾ���
    else if (m_currentOnceKey._X)
    {
        //�տ� ���⸦ ��� �ִٸ� �����Ѵ�
        //���Ⱑ �ֹ����, ��������Ŀ� ���� �ٸ� �ִϸ��̼��� �����Ѵ�. �켱 QBZ �ֹ��� Kar98k ��������
        if (inven.m_hand)
        {
            TAG_RES_STATIC tag = inven.m_hand->GetTagResStatic();
            if (tag == TAG_RES_STATIC::QBZ)
            {
                m_attacking = Attacking::Unarmed;
                //�ֹ��⸦ �����Ѵ�
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
                        false); //TODO: ������ �Ǵµ� �ٸ��� ���ڿ�������
                });
            }
            else if (tag == TAG_RES_STATIC::Kar98k)
            {
                m_attacking = Attacking::Unarmed;
                //�������⸦ �����Ѵ�
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
