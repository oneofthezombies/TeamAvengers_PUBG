#pragma once
#include "ComponentTransform.h"

struct SkinnedMesh;

class SkinnedMeshController : public Component
{
private:
    size_t m_currentIndex;
    float  m_totalBlendTime;
    float  m_passedBlendTime;

    SkinnedMesh* pSkinnedMesh;

private:
    void updateFrameToWorld(LPD3DXFRAME pFrameBase, LPD3DXFRAME pParent);
    void drawFrame(LPD3DXFRAME pFrameBase);
    void drawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase);

public:
             SkinnedMeshController(IObject* pOwner);
    virtual ~SkinnedMeshController();

    void Update(const function<void()>& function);
    void Render();

    void SetSkinnedMesh(SkinnedMesh* pSkinnedMesh);
    void SetAnimationIndex(
        const size_t index, const bool isBlend, 
        const float currentWeight = 1.0f, const float nextWeight = 0.0f, 
        const float blendTime = 0.3f);

    size_t GetCurrentAnimationIndex() const;
    size_t GetNumAnimation() const;

    Frame* FindFrame(const string& name);

private:
    /* do NOT use! this will be deleted soon.*/
    SkinnedMesh * loadSkinnedMesh(const string& path, const string& xFilename);

public:
    /* do NOT use! this will be deleted soon.*/
    void LoadAdditionalAnimation(const string& path, const string& xFilename);

    /* do NOT use! this will be deleted soon.*/
    void LoadSkinnedMesh(const string& path, const string& xFilename);
};