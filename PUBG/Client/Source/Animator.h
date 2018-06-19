#pragma once
#include "ComponentTransform.h"

class IObject;

class Animator : public Component
{
private:
    SkinnedMesh* m_pSkinnedMesh;
    int   m_Index;
    float m_BlendTime;
    float m_PassedBlendTime;

private:
    void UpdateAnim(const float dt);
    void UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

public:
    Animator(IObject* pOwner);
    virtual ~Animator();

    void Update();

    void SetAnimationIndex(const unsigned int index, const bool isBlend);
    void SetSkinnedMesh(SkinnedMesh* p);
    LPD3DXFRAME GetRootFrame();
};