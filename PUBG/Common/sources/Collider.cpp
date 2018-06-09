#include "stdafx.h"
#include "Collider.h"

Collider::Collider(Object* pOwner, const Type type)
    : Component(pOwner)
    , m_vCenter(0.0f, 0.0f, 0.0f)
    , m_type(type)
    , m_color(D3DCOLOR_XRGB(0, 255, 0))
    , m_pListener(nullptr)
    , m_tag(CollisionTag::kIdle)
{
    g_pCollisionManager->AddColliderBase(this);
}

Collider::~Collider()
{
    g_pCollisionManager->RemoveColliderBase(this);
}

void Collider::Render()
{
}

Collider::Type Collider::GetType() const
{
    return m_type;
}

void Collider::SetColor(const D3DCOLOR color)
{
    m_color = color;
}

D3DXVECTOR3 Collider::GetCenter() const
{
    return m_vCenter;
}

void Collider::SetListener(ICollisionListener& Listener)
{
    m_pListener = &Listener;
}

ICollisionListener* Collider::GetListener() const
{
    return m_pListener;
}

void Collider::SetTag(const CollisionTag tag)
{
    m_tag = tag;
}

CollisionTag Collider::GetTag() const
{
    return m_tag;
}

SphereCollider::SphereCollider(Object* owner)
    : Collider(owner, Collider::Type::kSphere)
{
}

void SphereCollider::Init(const float radius)
{
}

void SphereCollider::Update(const D3DXMATRIX& transform)
{
}

void SphereCollider::Render()
{
}

BoxCollider::BoxCollider(Object* owner)
    : Collider(owner, Collider::Type::kBox)
    , m_vExtent(0.0f, 0.0f, 0.0f)
    , m_mTransform(0.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 0.0f)
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
    m_vCenter = (min + max) / 2.0f;
    m_vExtent = max - m_vCenter;
    D3DXMatrixIdentity(&m_mTransform);

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

void BoxCollider::Update(const D3DXMATRIX& transform)
{
    D3DXMATRIX InverseMatrixOfCurrent, TM;
    D3DXMatrixInverse(&InverseMatrixOfCurrent, nullptr, &m_mTransform);
    TM = InverseMatrixOfCurrent * transform;
    m_mTransform = transform;
    D3DXVec3TransformCoord(&m_vCenter, &m_vCenter, &TM);
}

void BoxCollider::Render()
{
    vector<WORD> indices =
    {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7,
    };

    const auto dv = g_pDevice;
    dv->SetTransform(D3DTS_WORLD, &m_mTransform);
    dv->SetTexture(0, nullptr);
    dv->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, m_vertices.size(), indices.size() / 2, indices.data(), D3DFMT_INDEX16, m_vertices.data(), sizeof VERTEX_PC);
}

D3DXVECTOR3 BoxCollider::GetExtent() const
{
    return m_vExtent;
}

const D3DXMATRIX& BoxCollider::GetTransform() const
{
    return m_mTransform;
}

ICollisionListener::ICollisionListener(Object* pOwner)
    : Component(pOwner)
{
}

ICollisionListener::~ICollisionListener()
{
}
