#include "stdafx.h"
#include "Character.h"

/*
무기 장착 및 해제 애니메이션
*/
void Character::setRifleOnHand(TAG_RIFLE tagRifle)
{
    //총구 뒤쪽 카메라
    // OOTZ FLAG
    //m_info.pGunBolt = m_totalInventory.m_pHand->GetGunBolt();

    //애니메이션 정하기
    TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
    if (tagRifle == TAG_RIFLE::Primary)        //주무기 일 때
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot_OnHand;
        m_totalInventory.m_handState = TAG_RIFLE::Primary;
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //보조무기 일 때
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot_OnHand;
        m_totalInventory.m_handState = TAG_RIFLE::Secondary;
    }

    assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::setRifleOnHand(), tagAnim is COUNT");

    //애니메이션 적용
    setAnimation(
        CharacterAnimation::BodyPart::UPPER,
        tagAnim,
        false, //손에 들때 보간하면 총이 들리는 느낌이 나서 false
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
    //애니메이션 정하기
    TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
    if (tagRifle == TAG_RIFLE::Primary)        //주무기 일 때
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot_OnHand;
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //보조무기 일 때
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot_OnHand;
    }

    assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "setRifleOnHandCharacter(), tagAnim is COUNT");

    //애니메이션 적용
    pAnimation->Set(
        CharacterAnimation::BodyPart::UPPER,
        tagAnim,
        false, //손에 들때 보간하면 총이 들리는 느낌이 나서 false
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
    //애니메이션 정하기
    TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
    if (tagRifle == TAG_RIFLE::Primary)        //주무기 일 때
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot_OnHand;
    }
    else if (tagRifle == TAG_RIFLE::Secondary) //보조무기 일 때
    {
        if (m_stance == Stance::Stand || m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::Rifle_Combat_Prone_SecondarySlot_OnHand;
    }

    assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "setRifleOnHandEquip(), tagAnim is COUNT");

    //애니메이션 적용
    setEquipAnimation(
        CharacterAnimation::BodyPart::UPPER,
        tagAnim,
        false, //손에 들때 보간하면 총이 들리는 느낌이 나서 false
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