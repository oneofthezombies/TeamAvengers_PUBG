#include "stdafx.h"
#include "CharacterAnimation.h"
#include "SkinnedMeshController.h"
#include "ResourceInfo.h"

const float CharacterAnimation::DEFAULT_BLENDING_TIME = 0.3f;
const float CharacterAnimation::DEFAULT_NEXT_WEIGHT = 0.0f;

CharacterAnimation::CharacterAnimation()
    : IObject()
{
    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
    pSkinnedMeshController->SetSkinnedMesh(ResourceInfo::GetCharacterPathFileName());
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("pelvis"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_hand_root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_aim_root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_foot_root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("camera_tpp"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_target_root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_target_l"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_target_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_foot_l"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_foot_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_aim_l"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_aim_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_hand_gun"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_hand_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_hand_l"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("spine_01"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_l_01"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_f_01"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_b_01"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_r_01"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_r_02"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_r_03"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_b_02"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_b_03"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_f_02"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_f_03"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_l_02"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("skirt_l_03"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_twist_01_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("slot_sidearm"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("foot_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_twist_01_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ball_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_twist_01_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("foot_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_twist_01_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ball_l"));
}

CharacterAnimation::~CharacterAnimation()
{
}

void CharacterAnimation::OnUpdate()
{
}

void CharacterAnimation::OnRender()
{
}

void CharacterAnimation::UpdateAnimation()
{
    pSkinnedMeshController->UpdateAnimation();
}

void CharacterAnimation::UpdateModel()
{
    pSkinnedMeshController->UpdateModel();
}

void CharacterAnimation::Render(
    const D3DXMATRIX& world, 
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    pSkinnedMeshController->Render(world, setGlobalVariable);
}

void CharacterAnimation::Set(
    const BodyPart part,
    const TAG_ANIM_CHARACTER tag,
    const bool isBlend,
    const float blendingTime,
    const float nextWeight)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime, nextWeight);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime, nextWeight);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime, nextWeight);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime, nextWeight);
    }
}

void CharacterAnimation::Set(
    const BodyPart part,
    const TAG_ANIM_CHARACTER tag,
    const bool isBlend,
    const float loopEventPeriod,
    const std::function<void()>& loopEvent)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            loopEventPeriod,
            loopEvent);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            loopEventPeriod,
            loopEvent);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            loopEventPeriod,
            loopEvent);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            loopEventPeriod,
            loopEvent);
    }
}

void CharacterAnimation::Set(
    const BodyPart part,
    const TAG_ANIM_CHARACTER tag,
    const bool isBlend,
    const std::function<void()>& finishEvent)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            finishEvent);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            finishEvent);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            finishEvent);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            finishEvent);
    }
}

void CharacterAnimation::Set(
    const BodyPart part,
    const TAG_ANIM_CHARACTER tag,
    const bool isBlend,
    const float loopEventPeriod,
    const std::function<void()>& loopEvent,
    const std::function<void()>& finishEvent)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            loopEventPeriod,
            loopEvent,
            finishEvent);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            loopEventPeriod,
            loopEvent,
            finishEvent);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            loopEventPeriod,
            loopEvent,
            finishEvent);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            DEFAULT_BLENDING_TIME,
            DEFAULT_NEXT_WEIGHT,
            loopEventPeriod,
            loopEvent,
            finishEvent);
    }
}

Frame* CharacterAnimation::FindFrame(const string& name)
{
    return pSkinnedMeshController->FindFrame(name);
}

const string& CharacterAnimation::GetUpperAnimationName() const
{
    return pSkinnedMeshController->GetSubAnimationName();
}

const string& CharacterAnimation::GetLowerAnimationName() const
{
    return pSkinnedMeshController->GetAnimationName();
}

bool CharacterAnimation::HasUpperFinishEvent() const
{
    return pSkinnedMeshController->HasSubFinishEvent();
}

bool CharacterAnimation::HasLowerFinishEvent() const
{
    return pSkinnedMeshController->HasFinishEvent();
}
