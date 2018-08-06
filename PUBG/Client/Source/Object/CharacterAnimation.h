#pragma once
#include "IObject.h"

class SkinnedMeshController;

class CharacterAnimation : public IObject
{
public:
    enum class BodyPart
    {
        UPPER,
        LOWER,
        BOTH
    };

    static const float DEFAULT_BLENDING_TIME;
    static const float DEFAULT_NEXT_WEIGHT;
    static const float DEFAULT_FINISH_EVENT_AGO_TIME;
    static const float DEFAULT_POSITION;

private:
    SkinnedMeshController* pSkinnedMeshController;

public:
    CharacterAnimation(const std::size_t index);
    virtual ~CharacterAnimation();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void UpdateAnimation();
    void UpdateModel();
    void Render(
        const D3DXMATRIX& world, 
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

    void Set(
        const BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend = true,
        const float blendingTime = 0.3f,
        const float nextWeight = 0.0f,
        const float position = 0.0f);

    void Set(
        const BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void Set(
        const BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float loopEventPeriod,
        const std::function<void()>& loopEvent,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    Frame* FindFrame(const string& name);

    const string& GetUpperAnimationName() const;
    const string& GetLowerAnimationName() const;

    bool HasUpperFinishEvent() const;
    bool HasLowerFinishEvent() const;

    void GetUpperTrackDescription(
        const std::size_t index,
        D3DXTRACK_DESC* OutDesc);

    void GetLowerTrackDescription(
        const std::size_t index,
        D3DXTRACK_DESC* OutDesc);

    float GetUpperTrackPeriod(const std::size_t index);
    float GetLowerTrackPeriod(const std::size_t index);

    std::vector<BoundingSphere> GetBoundingSpheres();

    SkinnedMesh* GetSkinnedMesh();
};