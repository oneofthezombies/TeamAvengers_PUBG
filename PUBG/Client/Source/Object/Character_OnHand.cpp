#include "stdafx.h"
#include "Character.h"

/*
���� ���� �� ���� �ִϸ��̼�
*/
void Character::setRifleOnHand(TAG_RIFLE tagRifle)
{
    //�ѱ� ���� ī�޶�
    // OOTZ FLAG
    //m_info.pGunBolt = m_totalInventory.m_pHand->GetGunBolt();

    //�ִϸ��̼� ���ϱ�
    TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
    if (tagRifle == TAG_RIFLE::Primary)        //�ֹ��� �� ��
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot_OnHand;
        m_totalInventory.m_handState = TAG_RIFLE::Primary;
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //�������� �� ��
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot_OnHand;
        m_totalInventory.m_handState = TAG_RIFLE::Secondary;
    }

    assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnHand(), tagAnim is COUNT");

    //�ִϸ��̼� ����
    setAnimation(
        CharacterAnimation::BodyPart::UPPER,
        tagAnim,
        false, //�տ� �鶧 �����ϸ� ���� �鸮�� ������ ���� false
        CharacterAnimation::DEFAULT_BLENDING_TIME,
        CharacterAnimation::DEFAULT_NEXT_WEIGHT,
        CharacterAnimation::DEFAULT_POSITION,
        CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
        [this]()
    {
        setAnimation(
            CharacterAnimation::BodyPart::BOTH,
            m_lowerAnimState,
            false);
    });
}

void Character::setRifleOnHandCharacter(TAG_RIFLE tagRifle)
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

    assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "setRifleOnHandCharacter(), tagAnim is COUNT");

    //�ִϸ��̼� ����
    pAnimation->Set(
        CharacterAnimation::BodyPart::UPPER,
        tagAnim,
        false, //�տ� �鶧 �����ϸ� ���� �鸮�� ������ ���� false
        CharacterAnimation::DEFAULT_BLENDING_TIME,
        CharacterAnimation::DEFAULT_NEXT_WEIGHT,
        CharacterAnimation::DEFAULT_POSITION,
        CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
        [this]()
    {
        m_hasChangingState = false;
        pAnimation->Set(
            CharacterAnimation::BodyPart::BOTH,
            m_lowerAnimState,
            false);
    });
}

void Character::setRifleOnHandEquip(TAG_RIFLE tagRifle)
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

    assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "setRifleOnHandEquip(), tagAnim is COUNT");

    //�ִϸ��̼� ����
    setEquipAnimation(
        CharacterAnimation::BodyPart::UPPER,
        tagAnim,
        false, //�տ� �鶧 �����ϸ� ���� �鸮�� ������ ���� false
        CharacterAnimation::DEFAULT_BLENDING_TIME,
        CharacterAnimation::DEFAULT_NEXT_WEIGHT,
        CharacterAnimation::DEFAULT_POSITION,
        CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME,
        [this]()
    {
        setEquipAnimation(
            CharacterAnimation::BodyPart::BOTH,
            m_lowerAnimState,
            false);
    });
}