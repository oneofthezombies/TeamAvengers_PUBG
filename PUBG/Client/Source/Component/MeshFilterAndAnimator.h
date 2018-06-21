#pragma once
#include "ComponentTransform.h"

class IObject;

class MeshFilter : public Component
{
private:
    SkinnedMesh* m_pSkinnedMesh;

    void UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

public:
    MeshFilter(IObject* pOwner);
    virtual ~MeshFilter();

    void Update();

    void SetSkinnedMesh(SkinnedMesh* p);
    void SetSkinnedMesh(const string& path, const string& xFilename);

    LPD3DXFRAME GetRootFrame();
    LPD3DXANIMATIONCONTROLLER GetAnimationController();

    Frame* FindFrame(const string& name);
};

class Animator : public Component
{
private:
    MeshFilter* m_pMeshFilter;
    int   m_Index;
    float m_BlendTime;
    float m_PassedBlendTime;

private:
    void UpdateAnim(const float dt, LPD3DXANIMATIONCONTROLLER pAnimController);

public:
    Animator(IObject* pOwner);
    virtual ~Animator();

    void Update();

    void SetAnimationIndex(const unsigned int index, const bool isBlend);
};