#include "stdafx.h"
#include "ComponentTransform.h"
#include "IObject.h"

Component::Component(IObject* pOwner)
    : pOwner(pOwner)
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

Transform* Component::GetOwnerTransform() const
{
    assert(pOwner && "Component::GetTransform() is failed. owner is null.");

	return pOwner->GetTransform();
}

Transform::Transform(IObject* pOwner)
	: Component(pOwner)
	, m_Position(0.0f, 0.0f, 0.0f)
	, m_Rotation(0.0f, 0.0f, 0.0f, 0.0f)
	, m_Scale(1.0f, 1.0f, 1.0f)
{
}

Transform::~Transform()
{
}

const D3DXMATRIX& Transform::GetTransformationMatrix()
{
    D3DXMATRIX s, r, t;
    D3DXMatrixScaling(&s, m_Scale.x, m_Scale.y, m_Scale.z);

    D3DXMatrixRotationQuaternion(&r, &m_Rotation);

    D3DXMatrixTranslation(&t, m_Position.x, m_Position.y, m_Position.z);

    m_Transform = s * r * t;
    return m_Transform;
}

void Transform::SetPosition(const D3DXVECTOR3& pos)
{
    m_Position = pos;
}

const D3DXVECTOR3& Transform::GetPosition() const
{
    return m_Position;
}

void Transform::SetRotation(const D3DXQUATERNION& rot)
{
    m_Rotation = rot;
}

const D3DXQUATERNION& Transform::GetRotation() const
{
    return m_Rotation;
}

void Transform::SetScale(const D3DXVECTOR3& s)
{
    m_Scale = s;
}

const D3DXVECTOR3& Transform::GetScale() const
{
    return m_Scale;
}
