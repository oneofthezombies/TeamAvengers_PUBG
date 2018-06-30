#include "stdafx.h"
#include "Collider.h"
#include "CollisionManager.h"

vector<WORD> BoxCollider::s_indices = 
{
    0, 1, 1, 2, 2, 3, 3, 0,
    4, 5, 5, 6, 6, 7, 7, 4,
    0, 4, 1, 5, 2, 6, 3, 7,
};
vector<WORD> BoxCollider::f_indices = {
    6,2,2,3,3,7,7,6,
    2,0,0,1,1,3,
    1,5,5,7,
    0,4,4,6,
    5,4
};

Collider::Collider(IObject* pOwner, const TYPE type)
    : Component(pOwner)

    , m_center(Vector3::ZERO)
    , m_typeCollider(type)
    , m_tagCollision(TAG_COLLISION::Idle)
    , m_color(D3DCOLOR_XRGB(0, 255, 0))
{
    assert(pOwner && "Collider::Constructor() failed. owner is null.");

    Collision()()->AddCollider(this);
}

Collider::~Collider()
{
    Collision()()->RemoveCollider(this);
}

Collider::TYPE Collider::GetType() const
{
    return m_typeCollider;
}

const D3DXVECTOR3& Collider::GetCenter() const
{
    return m_center;
}

void Collider::SetTag(const TAG_COLLISION tag)
{
    m_tagCollision = tag;
}

TAG_COLLISION Collider::GetTag() const
{
    return m_tagCollision;
}

void Collider::AddOnCollisionEnterCallback(
    const onCollisionCallback_t& callback)
{
    onCollisionEnterCallbacks.emplace_back(callback);
}

void Collider::AddOnCollisionStayCallback(
    const onCollisionCallback_t& callback)
{
    onCollisionStayCallbacks.emplace_back(callback);
}

void Collider::AddOnCollisionExitCallback(
    const onCollisionCallback_t& callback)
{
    onCollisionExitCallbacks.emplace_back(callback);
}

const deque<onCollisionCallback_t>& 
    Collider::GetOnCollisionEnterCallbacks() const
{
    return onCollisionEnterCallbacks;
}

const deque<onCollisionCallback_t>& 
    Collider::GetOnCollisionStayCallbacks() const
{
    return onCollisionStayCallbacks;
}

const deque<onCollisionCallback_t>& 
    Collider::GetOnCollisionExitCallbacks() const
{
    return onCollisionExitCallbacks;
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
    , m_extent(0.0f, 0.0f, 0.0f)
{
    D3DXMatrixIdentity(&m_transformationMatrix);

    pEffect = Resource()()->GetEffect("./Resource/", "Color.fx");
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
    m_center = (min + max) * 0.5f;
    m_extent = max - m_center;

    m_vertices.resize(8);
    m_vertices[0] = D3DXVECTOR3(min.x, min.y, min.z);
    m_vertices[1] = D3DXVECTOR3(min.x, max.y, min.z);
    m_vertices[2] = D3DXVECTOR3(max.x, max.y, min.z);
    m_vertices[3] = D3DXVECTOR3(max.x, min.y, min.z);
    m_vertices[4] = D3DXVECTOR3(min.x, min.y, max.z);
    m_vertices[5] = D3DXVECTOR3(min.x, max.y, max.z);
    m_vertices[6] = D3DXVECTOR3(max.x, max.y, max.z);
    m_vertices[7] = D3DXVECTOR3(max.x, min.y, max.z);
}

void BoxCollider::Init(const D3DXMATRIX& transform)
{
    Init(Vector3::ONE * -0.5f, Vector3::ONE * 0.5f);
    Update(transform);
}

void BoxCollider::Update(const D3DXMATRIX& transform)
{
    D3DXMATRIX InverseMatrixOfCurrent, TM;
    D3DXMatrixInverse(
        &InverseMatrixOfCurrent, nullptr, &m_transformationMatrix);
    TM = InverseMatrixOfCurrent * transform;
    D3DXVec3TransformCoord(&m_center, &m_center, &TM);

    m_transformationMatrix = transform;
}

const D3DXVECTOR3& BoxCollider::GetExtent() const
{
    return m_extent;
}

const D3DXMATRIX& BoxCollider::GetTransformationMatrix() const
{
    return m_transformationMatrix;
}

void BoxCollider::Render()
{
    Shader::Draw(
        pEffect, 
        nullptr, 
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(Shader::World, &m_transformationMatrix);
        D3DXCOLOR green(0.0f, 1.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &green, sizeof green);
    }, 
        [this]() 
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_LINELIST,
            0,
            m_vertices.size(),
            s_indices.size() / 2,
            s_indices.data(),
            D3DFMT_INDEX16,
            m_vertices.data(),
            sizeof m_vertices.front());
    });
}