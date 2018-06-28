#include "stdafx.h"
#include "TerrainFeature.h"
#include "Collider.h"
#include "EffectMeshRenderer.h"

TerrainFeature::TerrainFeature(
    const TAG_RES_STATIC tag,
    const D3DXVECTOR3& position,
    const D3DXVECTOR3& rotation,
    const D3DXVECTOR3& scale)
    : IObject()
    , pEffectMeshRenderer(nullptr)
{
    pEffectMeshRenderer = AddComponent<EffectMeshRenderer>();
    pEffectMeshRenderer->SetEffectMesh(tag);

    Transform* tr = GetTransform();
    tr->SetPosition(position);
    tr->SetRotation(rotation);
    tr->SetScale(scale);
}

TerrainFeature::~TerrainFeature()
{
    for (auto c : m_colliders)
        SAFE_DELETE(c);
}

void TerrainFeature::OnUpdate()
{
    // do nothing
}

void TerrainFeature::OnRender()
{
    pEffectMeshRenderer->Render();

    for (auto c : m_colliders)
        c->Render();
}

void TerrainFeature::AddBoxCollider(const D3DXMATRIX& transform)
{
    BoxCollider* pBoxCollider = new BoxCollider(this);
    pBoxCollider->Init(transform);
    pBoxCollider->SetTag(TAG_COLLISION::Impassable);

    m_colliders.emplace_back(pBoxCollider);
}
