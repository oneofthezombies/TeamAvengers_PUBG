#pragma once
#include "ComponentTransform.h"

class IColliderRenderer : public Component
{
protected:
    D3DCOLOR m_color;

    IColliderRenderer(IObject* pOwner);

public:
    virtual ~IColliderRenderer();

    void SetColor(const D3DCOLOR color);

    virtual void Render() = 0;
};

class BoxColliderRenderer : public IColliderRenderer
{
private:
    vector<VERTEX_PC> m_vertices;

public:
    BoxColliderRenderer(IObject* pOwner);
    virtual ~BoxColliderRenderer();

    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
    virtual void Render() override;
};

class SphereColliderRenderer : public IColliderRenderer
{
public:
    SphereColliderRenderer(IObject* pOwner);
    virtual ~SphereColliderRenderer();

    virtual void Render() override;
};