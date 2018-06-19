#pragma once
#include "ComponentTransform.h"
#include "TagRenderer.h"

class IObject;
class Animator;

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

class ColliderRenderer : public Renderer
{
protected:
    D3DCOLOR m_color;

    ColliderRenderer(IObject* pOwner, const TAG_RENDERER tag);

public:
    virtual ~ColliderRenderer();

    virtual void Render() override;

    void SetColor(const D3DCOLOR color);
};

class BoxColliderRenderer : public ColliderRenderer
{
private:
    vector<VERTEX_PC> m_vertices;

public:
    BoxColliderRenderer(IObject* pOwner);
    virtual ~BoxColliderRenderer();

    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    virtual void Render() override;
};
