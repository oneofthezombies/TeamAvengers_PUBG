#include "stdafx.h"
#include "CollidableStaticObject.h"
#include "Collider.h"
#include "EffectMeshRenderer.h"

CollidableStaticObject::CollidableStaticObject(
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

CollidableStaticObject::~CollidableStaticObject()
{
    for (auto c : m_colliders)
        SAFE_DELETE(c);
}

void CollidableStaticObject::OnUpdate()
{
    // do nothing
}

void CollidableStaticObject::OnRender()
{
    pEffectMeshRenderer->Render();

    for (auto c : m_colliders)
        c->Render();
}

void CollidableStaticObject::AddBoxCollider(
    const D3DXVECTOR3& center, 
    const D3DXVECTOR3& extent, 
    const D3DXMATRIX& transform)
{
    BoxCollider* pBoxCollider = new BoxCollider(this);
    pBoxCollider->Init(center, extent, transform);

    m_colliders.emplace_back(pBoxCollider);
}
