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

private:
    SkinnedMeshController* pSkinnedMeshController;

public:
    CharacterAnimation();
    virtual ~CharacterAnimation();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void UpdateAnimation();
    void UpdateModel();
    void Render(const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

    void Set(
        const BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend = true,
        const float blendingTime = 0.3f,
        const float nextWeight = 0.0f);

    void Set(
        const BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float loopEventPeriod,
        const std::function<void()>& loopEvent);

    void Set(
        const BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const std::function<void()>& finishEvent);

    void Set(
        const BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float loopEventPeriod,
        const std::function<void()>& loopEvent,
        const std::function<void()>& finishEvent);

    Frame* FindFrame(const string& name);

    const string& GetUpperAnimationName() const;
    const string& GetLowerAnimationName() const;

    bool HasUpperFinishEvent() const;
    bool HasLowerFinishEvent() const;
};