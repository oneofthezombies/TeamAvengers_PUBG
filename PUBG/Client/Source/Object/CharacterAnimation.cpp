#include "stdafx.h"
#include "CharacterAnimation.h"
#include "SkinnedMeshController.h"
#include "ResourceInfo.h"

const float CharacterAnimation::DEFAULT_BLENDING_TIME = 0.3f;
const float CharacterAnimation::DEFAULT_NEXT_WEIGHT = 0.0f;
const float CharacterAnimation::DEFAULT_POSITION = 0.0f;
const float CharacterAnimation::DEFAULT_FINISH_EVENT_AGO_TIME = 0.0f;

CharacterAnimation::CharacterAnimation()
    : IObject(TAG_OBJECT::Idle)
{
    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
    pSkinnedMeshController->SetSkinnedMesh(ResourceInfo::GetCharacterPathFileName());
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("pelvis"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_hand_root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_aim_root"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_foot_root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("camera_tpp"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_target_root"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_target_l"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_target_r"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_foot_l"));
    pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_foot_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_aim_l"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_aim_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_hand_gun"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_hand_r"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("ik_hand_l"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("spine_01"));
    //pSkinnedMeshController->AddAnimationBackupFrame(pSkinnedMeshController->FindFrame("spine_02"));
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
    const float nextWeight,
    const float position)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime, 
            nextWeight,
            position);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime, 
            nextWeight,
            position);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime, 
            nextWeight,
            position);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime, 
            nextWeight,
            position);
    }
}

void CharacterAnimation::Set(
    const BodyPart part, 
    const TAG_ANIM_CHARACTER tag, 
    const bool isBlend, 
    const float blendingTime, 
    const float nextWeight,
    const float position,
    const float finishEventAgoTime, 
    const std::function<void()>& finishEvent)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);
    }
}

void CharacterAnimation::Set(
    const BodyPart part, 
    const TAG_ANIM_CHARACTER tag, 
    const bool isBlend, 
    const float blendingTime, 
    const float nextWeight,
    const float position,
    const float loopEventPeriod, 
    const std::function<void()>& loopEvent, 
    const float finishEventAgoTime, 
    const std::function<void()>& finishEvent)
{
    if (part == BodyPart::UPPER)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            loopEventPeriod,
            loopEvent,
            finishEventAgoTime,
            finishEvent);
    }
    else if (part == BodyPart::LOWER)
    {
        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            loopEventPeriod,
            loopEvent,
            finishEventAgoTime,
            finishEvent);
    }
    else if (part == BodyPart::BOTH)
    {
        pSkinnedMeshController->SetAnimation(
            true,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            loopEventPeriod,
            loopEvent,
            finishEventAgoTime,
            finishEvent);

        pSkinnedMeshController->SetAnimation(
            false,
            TagAnimation::GetString(tag),
            TagAnimation::GetSpeed(tag),
            isBlend,
            blendingTime,
            nextWeight,
            position,
            loopEventPeriod,
            loopEvent,
            finishEventAgoTime,
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

void CharacterAnimation::GetUpperTrackDescription(
    const std::size_t index,
    D3DXTRACK_DESC* OutDesc)
{
    pSkinnedMeshController->GetSubTrackDescription(index, OutDesc);
}

void CharacterAnimation::GetLowerTrackDescription(
    const std::size_t index,
    D3DXTRACK_DESC* OutDesc)
{
    pSkinnedMeshController->GetTrackDescription(index, OutDesc);
}

float CharacterAnimation::GetUpperTrackPeriod(const std::size_t index)
{
    return pSkinnedMeshController->GetSubTrackPeriod(index);
}

float CharacterAnimation::GetLowerTrackPeriod(const std::size_t index)
{
    return pSkinnedMeshController->GetTrackPeriod(index);
}

std::vector<BoundingSphere> CharacterAnimation::GetBoundingSpheres()
{
    return pSkinnedMeshController->GetBoundingSpheres();
}
