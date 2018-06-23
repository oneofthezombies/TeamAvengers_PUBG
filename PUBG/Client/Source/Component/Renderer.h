#pragma once
#include "ComponentTransform.h"
#include "TagRenderer.h"

class IObject;
class MeshFilter;
class BoxCollider;

class Renderer : public Component
{
private:
    const TAG_RENDERER m_Tag;

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
    EffectMesh* pEffectMesh;

public:
    EffectMeshRenderer(IObject* pOwner);
    virtual ~EffectMeshRenderer();

    virtual void Render() override;

    void SetEffectMesh(EffectMesh* p);
    void SetEffectMesh(const string& path, const string& xFilename);
};

class SkinnedMeshRenderer : public Renderer
{
private:
    MeshFilter* pMeshFilter;

    void drawFrame(LPD3DXFRAME pFrameBase);
    void drawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase);
    void printBoneNameAndPosition(LPD3DXFRAME pFrameBase);

public:
    SkinnedMeshRenderer(IObject* pOwner);
    virtual ~SkinnedMeshRenderer();

    virtual void Render() override;
};

class ColliderRenderer : public Renderer
{
protected:
    D3DCOLOR m_Color;

    ColliderRenderer(IObject* pOwner, const TAG_RENDERER tag);

public:
    virtual ~ColliderRenderer();

    virtual void Render() override;

    void SetColor(const D3DCOLOR color);
};

class BoxColliderRenderer : public ColliderRenderer
{
private:
    BoxCollider* pBoxCollider;
    vector<VERTEX_PC> m_Vertices;

public:
    BoxColliderRenderer(IObject* pOwner);
    virtual ~BoxColliderRenderer();

    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    virtual void Render() override;
};
