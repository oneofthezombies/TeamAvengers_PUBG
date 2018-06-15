#include "stdafx.h"
#include "ColliderRenderer.h"
#include "IObject.h"
#include "Collider.h"

IColliderRenderer::IColliderRenderer(IObject* pOwner)
    : Component(pOwner)
    , m_color(D3DCOLOR_XRGB(0, 255, 0))
{
}

IColliderRenderer::~IColliderRenderer()
{
}

void IColliderRenderer::SetColor(const D3DCOLOR color)
{
    m_color = color;
}

BoxColliderRenderer::BoxColliderRenderer(IObject* pOwner)
    : IColliderRenderer(pOwner)
{
}

BoxColliderRenderer::~BoxColliderRenderer()
{
}

void BoxColliderRenderer::Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
    m_vertices.resize(8);
    m_vertices[0] = VERTEX_PC(D3DXVECTOR3(min.x, min.y, min.z), m_color);
    m_vertices[1] = VERTEX_PC(D3DXVECTOR3(min.x, max.y, min.z), m_color);
    m_vertices[2] = VERTEX_PC(D3DXVECTOR3(max.x, max.y, min.z), m_color);
    m_vertices[3] = VERTEX_PC(D3DXVECTOR3(max.x, min.y, min.z), m_color);
    m_vertices[4] = VERTEX_PC(D3DXVECTOR3(min.x, min.y, max.z), m_color);
    m_vertices[5] = VERTEX_PC(D3DXVECTOR3(min.x, max.y, max.z), m_color);
    m_vertices[6] = VERTEX_PC(D3DXVECTOR3(max.x, max.y, max.z), m_color);
    m_vertices[7] = VERTEX_PC(D3DXVECTOR3(max.x, min.y, max.z), m_color);
}

void BoxColliderRenderer::Render()
{
    const auto bc = GetOwner()->GetComponent<BoxCollider>();
    if (!bc) return;

    vector<WORD> indices =
    {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7,
    };

    const auto dv = g_pDevice;
    dv->SetTransform(D3DTS_WORLD, &bc->GetTransform());
    dv->SetTexture(0, nullptr);
    dv->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, m_vertices.size(), indices.size() / 2, indices.data(), D3DFMT_INDEX16, m_vertices.data(), sizeof VERTEX_PC);
}

SphereColliderRenderer::SphereColliderRenderer(IObject* pOwner)
    : IColliderRenderer(pOwner)
{
}

SphereColliderRenderer::~SphereColliderRenderer()
{
}

void SphereColliderRenderer::Render()
{
}
