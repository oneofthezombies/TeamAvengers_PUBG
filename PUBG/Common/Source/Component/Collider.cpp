#include "stdafx.h"
#include "Collider.h"
#include "CollisionManager.h"

Collider::Collider(IObject* pOwner, const Type type)
    : Component(pOwner)
    , m_vCenter(0.0f, 0.0f, 0.0f)
    , m_type(type)
    , m_pListener(nullptr)
    , m_tag(TAG_COLLISION::IDLE)
{
    g_pCollisionManager->AddCollider(this);
}

Collider::~Collider()
{
    g_pCollisionManager->RemoveCollider(this);
}

Collider::Type Collider::GetType() const
{
    return m_type;
}

void Collider::SetCenter(const D3DXVECTOR3& center)
{
    m_vCenter = center;
}

D3DXVECTOR3 Collider::GetCenter() const
{
    return m_vCenter;
}

void Collider::SetListener(ICollisionListener* pListener)
{
    if (!pListener) return;

    m_pListener = pListener;
}

ICollisionListener* Collider::GetListener() const
{
    return m_pListener;
}

void Collider::SetTag(const TAG_COLLISION tag)
{
    m_tag = tag;
}

TAG_COLLISION Collider::GetTag() const
{
    return m_tag;
}

SphereCollider::SphereCollider(IObject* pOwner)
    : Collider(pOwner, Collider::Type::kSphere)
{
}

void SphereCollider::Init(const float radius)
{
}

void SphereCollider::Update(const D3DXMATRIX& transform)
{
}

BoxCollider::BoxCollider(IObject* pOwner)
    : Collider(pOwner, Collider::Type::kBox)
    , m_vExtent(0.0f, 0.0f, 0.0f)
{
    D3DXMatrixIdentity(&m_mTransform);
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
    m_vCenter = (min + max) * 0.5f;
    m_vExtent = max - m_vCenter;
}

void BoxCollider::Update(const D3DXMATRIX& transform)
{
    D3DXMATRIX InverseMatrixOfCurrent, TM;
    D3DXMatrixInverse(&InverseMatrixOfCurrent, nullptr, &m_mTransform);
    TM = InverseMatrixOfCurrent * transform;
    D3DXVec3TransformCoord(&m_vCenter, &m_vCenter, &TM);

    m_mTransform = transform;
}

void BoxCollider::SetExtent(const D3DXVECTOR3& extent)
{
    m_vExtent = extent;
}

D3DXVECTOR3 BoxCollider::GetExtent() const
{
    return m_vExtent;
}

const D3DXMATRIX& BoxCollider::GetTransform() const
{
    return m_mTransform;
}

ICollisionListener::ICollisionListener(IObject* pOwner)
    : Component(pOwner)
{
}

ICollisionListener::~ICollisionListener()
{
}
