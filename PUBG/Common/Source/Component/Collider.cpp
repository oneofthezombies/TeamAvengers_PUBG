#include "stdafx.h"
#include "Collider.h"
#include "CollisionManager.h"

Collider::Collider(IObject* pOwner, const TYPE type)
    : Component(pOwner)
    , m_Center(0.0f, 0.0f, 0.0f)
    , m_Type(type)
    , m_Tag(TAG_COLLISION::IDLE)

    , pListener(nullptr)
{
    assert(pOwner && "Collider::Constructor() failed. owner is null.");

    g_pCollisionManager->AddCollider(this);
}

Collider::~Collider()
{
    g_pCollisionManager->RemoveCollider(this);
}

Collider::TYPE Collider::GetType() const
{
    return m_Type;
}

void Collider::SetCenter(const D3DXVECTOR3& center)
{
    m_Center = center;
}

const D3DXVECTOR3& Collider::GetCenter() const
{
    return m_Center;
}

void Collider::SetListener(ICollisionListener* pListener)
{
    assert(pListener && "Collider::SetListener() failed. listener is null.");

    this->pListener = pListener;
}

ICollisionListener* Collider::GetListener() const
{
    return pListener;
}

void Collider::SetTag(const TAG_COLLISION tag)
{
    m_Tag = tag;
}

TAG_COLLISION Collider::GetTag() const
{
    return m_Tag;
}

SphereCollider::SphereCollider(IObject* pOwner)
    : Collider(pOwner, Collider::TYPE::SPHERE)
{
    assert(false && "No impl.");
}

void SphereCollider::Init(const float radius)
{
}

void SphereCollider::Update(const D3DXMATRIX& transform)
{
}

BoxCollider::BoxCollider(IObject* pOwner)
    : Collider(pOwner, Collider::TYPE::BOX)
    , m_Extent(0.0f, 0.0f, 0.0f)
{
    D3DXMatrixIdentity(&m_Transform);
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
    m_Center = (min + max) * 0.5f;
    m_Extent = max - m_Center;
}

void BoxCollider::Update(const D3DXMATRIX& transform)
{
    D3DXMATRIX InverseMatrixOfCurrent, TM;
    D3DXMatrixInverse(&InverseMatrixOfCurrent, nullptr, &m_Transform);
    TM = InverseMatrixOfCurrent * transform;
    D3DXVec3TransformCoord(&m_Center, &m_Center, &TM);

    m_Transform = transform;
}

void BoxCollider::SetExtent(const D3DXVECTOR3& extent)
{
    m_Extent = extent;
}

const D3DXVECTOR3& BoxCollider::GetExtent() const
{
    return m_Extent;
}

const D3DXMATRIX& BoxCollider::GetTransform() const
{
    return m_Transform;
}

ICollisionListener::ICollisionListener(IObject* pOwner)
    : Component(pOwner)
{
    assert(pOwner && 
        "ICollisionListener::Constructor() failed. owner is null.");
}

ICollisionListener::~ICollisionListener()
{
}
