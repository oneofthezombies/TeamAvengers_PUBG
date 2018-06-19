#pragma once
#include "ComponentTransform.h"

class IObject;
class Animator;

enum class TAG_RENDERER
{
    DEFAULT,
    EFFECT_MESH,
    SKINNED_MESH
};

class Renderer : public Component
{
private:
    const TAG_RENDERER m_tag;

protected:
    Renderer(IObject* pOwner, const TAG_RENDERER tag);

public:
    virtual ~Renderer();

    virtual void Render() = 0;

    TAG_RENDERER GetTag() const;
};

class EffectMeshRenderer : public Renderer
{
private:
    EffectMesh* m_pEffectMesh;

public:
    EffectMeshRenderer(IObject* pOwner);
    virtual ~EffectMeshRenderer();

    virtual void Render() override;

    void SetEffectMesh(EffectMesh* p);
};

class SkinnedMeshRenderer : public Renderer
{
private:
    Animator* pAnimator;

    void DrawFrame(LPD3DXFRAME pFrame);
    void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainer);

public:
    SkinnedMeshRenderer(IObject* pOwner);
    virtual ~SkinnedMeshRenderer();

    virtual void Render() override;
};