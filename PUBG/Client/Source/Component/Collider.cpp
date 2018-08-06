#include "stdafx.h"
#include "Collider.h"
#include "Collision.h"

std::vector<WORD> BoxCollider::INDICES =
{
    0, 1, 1, 2, 2, 3, 3, 0,
    4, 5, 5, 6, 6, 7, 7, 4,
    0, 4, 1, 5, 2, 6, 3, 7,
};

std::vector<WORD> BoxCollider::FRUSTUM_INDICES = 
{
    6, 2, 2, 3, 3, 7, 7, 6,
    2, 0, 0, 1, 1, 3,
    1, 5, 5, 7,
    0, 4, 4, 6,
    5, 4
};

Collider::Collider(IObject* pOwner, const Type type)
    : Component(pOwner)

    , m_center(Vector3::ZERO)
    , m_typeCollider(type)
    , m_tagCollision(TAG_COLLISION::Idle)
    , m_color(D3DCOLOR_XRGB(0, 255, 0))
{
    assert(pOwner && "Collider::Constructor(), owner is null.");

    Collision()()->AddCollider(this);
}

Collider::~Collider()
{
    Collision()()->RemoveCollider(this);
}

Collider::Type Collider::GetTypeCollider() const
{
    return m_typeCollider;
}

const D3DXVECTOR3& Collider::GetCenter() const
{
    return m_center;
}

void Collider::SetTagCollision(const TAG_COLLISION tag)
{
    m_tagCollision = tag;
}

TAG_COLLISION Collider::GetTagCollision() const
{
    return m_tagCollision;
}

void Collider::AddOnCollisionEnterCallback(
    const on_collision_callback_t& callback)
{
    m_onCollisionEnterCallbacks.emplace_back(callback);
}

void Collider::AddOnCollisionStayCallback(
    const on_collision_callback_t& callback)
{
    m_onCollisionStayCallbacks.emplace_back(callback);
}

void Collider::AddOnCollisionExitCallback(
    const on_collision_callback_t& callback)
{
    m_onCollisionExitCallbacks.emplace_back(callback);
}

const on_collision_callbacks_t&
    Collider::GetOnCollisionEnterCallbacks() const
{
    return m_onCollisionEnterCallbacks;
}

const on_collision_callbacks_t&
    Collider::GetOnCollisionStayCallbacks() const
{
    return m_onCollisionStayCallbacks;
}

const on_collision_callbacks_t&
    Collider::GetOnCollisionExitCallbacks() const
{
    return m_onCollisionExitCallbacks;
}

SphereCollider::SphereCollider(IObject* pOwner)
    : Collider(pOwner, Collider::Type::SPHERE)
{
    assert(false && "SphereCollider::Constructor(), No impl.");
}

void SphereCollider::Init(const float radius)
{
}

void SphereCollider::Update(const D3DXMATRIX& transform)
{
}

BoxCollider::BoxCollider(IObject* pOwner)
    : Collider(pOwner, Collider::Type::BOX)
    , m_extent(Vector3::ZERO)
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

void BoxCollider::Init(const D3DXMATRIX& transformationMatrix)
{
    D3DXVECTOR3 extent(Vector3::ONE * 0.5f);

    D3DXVECTOR3 vs;
    D3DXQUATERNION qr;
    Matrix::GetScaleAndRotation(transformationMatrix, &vs, &qr);
    D3DXVECTOR3 vt = Matrix::GetTranslation(transformationMatrix);

    D3DXMATRIX s;
    D3DXMatrixScaling(&s, vs.x, vs.y, vs.z);
    D3DXVec3TransformCoord(&extent, &extent, &s);

    Init(-extent, extent);

    D3DXMATRIX invS;
    D3DXMatrixScaling(&invS, vs.x, vs.y, vs.z);
    D3DXMatrixInverse(&invS, nullptr, &invS);
    Update(invS * transformationMatrix);
}

void BoxCollider::Update(const D3DXMATRIX& transformationMatrix)
{
    D3DXMATRIX invCurrent, variance;

    D3DXMatrixInverse(
        &invCurrent,
        nullptr, 
        &m_transformationMatrix);

    variance = invCurrent * transformationMatrix;

    D3DXVec3TransformCoord(&m_center, &m_center, &variance);

    m_transformationMatrix = transformationMatrix;
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
        pEffect->SetMatrix(Shader::World, &(m_transformationMatrix));
        D3DXCOLOR green(0.0f, 1.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &green, sizeof green);
    }, 
        [this]() 
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_LINELIST,
            0,
            static_cast<UINT>(m_vertices.size()),
            static_cast<UINT>(INDICES.size() / 2),
            INDICES.data(),
            D3DFMT_INDEX16,
            m_vertices.data(),
            sizeof m_vertices.front());
    });
}
