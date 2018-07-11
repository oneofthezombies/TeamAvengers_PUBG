#pragma once
#include "ComponentTransform.h"

#define NO_ANIMATION_FINISH_EVENT -1.0f

using animation_event_t  = std::pair<float, std::function<void()>>;
using animation_events_t = std::deque<animation_event_t>;

class SkinnedMeshController : public Component
{
private:
    SkinnedMesh*         pSkinnedMesh;

    std::string          m_animName;
    float                m_totalBlendingTime;
    float                m_passedBlendingTime;
    animation_events_t   m_finishEvents;
    animation_events_t   m_loopEvents;

    std::string          m_subAnimName;
    float                m_subTotalBlendingTime;
    float                m_subPassedBlendingTime;
    animation_events_t   m_subFinishEvents;
    animation_events_t   m_subLoopEvents;

    std::vector<std::pair<Frame*, D3DXMATRIX>> m_animationBackup;

private:
    void updateAnimation(
        const float dt,
        const float totalBlendingTime,
        LPD3DXANIMATIONCONTROLLER pController,
        float* OutPassedBlendingTime);

    void notifyAnimationEvent(
        const float dt,
        const std::string& name,
        LPD3DXANIMATIONCONTROLLER pController,
        animation_events_t* OutLoopEvents,
        animation_events_t* OutFinishEvents);

    void updateFrameToModelSpace(LPD3DXFRAME pFrameBase, LPD3DXFRAME pParent);
    
    void drawFrame(
        LPD3DXFRAME pFrameBase, 
        const D3DXMATRIX& world,
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

    void drawMeshContainer(
        LPD3DXMESHCONTAINER pMeshContainerBase,
        const D3DXMATRIX& world,
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

    void findBoundingSphere(
        LPD3DXFRAME pFrame, 
        std::vector<BoundingSphere>* OutBoundingSpheres);
    void findBoundingSphere(
        LPD3DXMESHCONTAINER pMeshContainer, 
        std::vector<BoundingSphere>* OutBoundingSpheres);

    void findMesh(
        LPD3DXFRAME pFrame, 
        std::vector<std::pair<LPD3DXMESH, std::size_t>>* OutMeshs);
    void findMesh(
        LPD3DXMESHCONTAINER pMeshContainer, 
        std::vector<std::pair<LPD3DXMESH, std::size_t>>* OutMeshs);

public:
             SkinnedMeshController(IObject* pOwner);
    virtual ~SkinnedMeshController();

    void UpdateAnimation();
    void UpdateModel();
    void UpdateMesh();
    void Render(
        const D3DXMATRIX& world, 
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

            void SetSkinnedMesh(SkinnedMesh* pSkinnedMesh);
            void SetSkinnedMesh(
                const std::pair<std::string, std::string>& path, 
                const std::size_t index);
    SkinnedMesh* GetSkinnedMesh() const;

    void SetAnimation(
        const bool isSub,
        const std::string& name,
        const float nextSpeed = 1.0f,
        const bool isBlend = true,
        const float blendingTime = 0.3f,
        const float nextWeight = 0.0f,
        const float position = 0.0f);

    void SetAnimation(
        const bool isSub,
        const std::string& name,
        const float nextSpeed,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void SetAnimation(
        const bool isSub,
        const std::string& name,
        const float nextSpeed,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float loopEventPeriod,
        const std::function<void()>& loopEvent,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent = []() {});

    Frame* FindFrame(const string& name);

    void AddAnimationBackupFrame(Frame* pFrame);

    const std::string& GetAnimationName() const;
          std::size_t  GetNumAnimation()  const;

    const std::string& GetSubAnimationName() const;
          std::size_t  GetSubNumAnimation()  const;

    bool HasFinishEvent()    const;
    bool HasSubFinishEvent() const;

    void GetTrackDescription(
        const std::size_t index, 
        D3DXTRACK_DESC* OutDesc);

    void GetSubTrackDescription(
        const std::size_t index, 
        D3DXTRACK_DESC* OutDesc);

    float GetTrackPeriod(const std::size_t index);
    float GetSubTrackPeriod(const std::size_t index);

    std::vector<BoundingSphere> GetBoundingSpheres();
};