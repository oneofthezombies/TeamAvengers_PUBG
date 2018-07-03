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
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("root"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("pelvis"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_twist_01_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("thigh_twist_01_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_twist_01_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("calf_twist_01_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("foot_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("foot_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ball_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ball_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("slot_sidearm"));
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

void CharacterAnimation::Render(const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    pSkinnedMeshController->Render(setGlobalVariable);
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
