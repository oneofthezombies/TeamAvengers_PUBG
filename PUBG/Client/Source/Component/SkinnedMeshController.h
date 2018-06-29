#pragma once
#include "ComponentTransform.h"

struct SkinnedMeshInstance;

class SkinnedMeshController : public Component
{
private:
    size_t m_currentAnimIndex;
    string m_currentAnimName;

    float  m_totalBlendTime;
    float  m_passedBlendTime;

    SkinnedMeshInstance* m_pSkinnedMeshInstance;

private:
    void updateFrameToModelSpace(LPD3DXFRAME pFrameBase, LPD3DXFRAME pParent);
    void drawFrame(
        LPD3DXFRAME pFrameBase, 
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);
    void drawMeshContainer(
        LPD3DXMESHCONTAINER pMeshContainerBase,
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

public:
             SkinnedMeshController(IObject* pOwner);
    virtual ~SkinnedMeshController();

    void UpdateAnimation();
    void UpdateModel();
    void Render(const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

    void SetSkinnedMesh(SkinnedMesh* pSkinnedMesh);
    void SetAnimationIndex(
        const size_t index, const bool isBlend, 
        const float currentWeight = 1.0f, const float nextWeight = 0.0f, 
        const float blendTime = 0.3f);
    void SetAnimation(
        const string& name, const bool isBlend,
        const float currentWeight = 1.0f, const float nextWeight = 0.0f,
        const float blendTime = 0.3f);

          size_t  GetCurrentAnimationIndex() const;
    const string& GetCurrentAnimationName()  const;
          size_t  GetNumAnimation()          const;

    Frame* FindFrame(const string& name);
};