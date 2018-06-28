#include "stdafx.h"
#include "ComponentTransform.h"
#include "IObject.h"

Component::Component(IObject* pOwner)
    : MemoryAllocator()
    , pOwner(pOwner)
{
    assert(pOwner && "Component::Constructor() failed. owner is null.");
}

Component::~Component()
{
}

IObject* Component::GetOwner() const
{
    return pOwner;
}

Transform* Component::GetTransform() const
{
    assert(pOwner && "Component::GetTransform() is failed. owner is null.");

	return pOwner->GetTransform();
}

Transform::Transform(IObject* pOwner)
	: Component(pOwner)
	, m_position(Vector3::ZERO)
	, m_rotation(Quaternion::ZERO)
	, m_scale(Vector3::ONE)
{
}

Transform::~Transform()
{
}

void Transform::Update()
{
    D3DXMATRIX s, r, t;
    D3DXMatrixScaling(&s, m_scale.x, m_scale.y, m_scale.z);
    D3DXMatrixRotationQuaternion(&r, &m_rotation);
    D3DXMatrixTranslation(&t, m_position.x, m_position.y, m_position.z);

    m_transformationMatrix = s * r * t;
}

const D3DXMATRIX& Transform::GetTransformationMatrix()
{
    return m_transformationMatrix;
}

void Transform::SetPosition(const D3DXVECTOR3& pos)
{
    m_position = pos;
}

const D3DXVECTOR3& Transform::GetPosition() const
{
    return m_position;
}

void Transform::SetRotation(const D3DXQUATERNION& rot)
{
    m_rotation = rot;
}

void Transform::SetRotation(const D3DXVECTOR3& r)
{
    D3DXMATRIX m;
    D3DXMatrixRotationYawPitchRoll(&m, r.y, r.x, r.z);
    D3DXQuaternionRotationMatrix(&m_rotation, &m);
}

const D3DXQUATERNION& Transform::GetRotation() const
{
    return m_rotation;
}

void Transform::SetScale(const D3DXVECTOR3& s)
{
    m_scale = s;
}

const D3DXVECTOR3& Transform::GetScale() const
{
    return m_scale;
}

void Transform::SetTransform(const D3DXMATRIX& m)
{
    m_transformationMatrix = m;
}
