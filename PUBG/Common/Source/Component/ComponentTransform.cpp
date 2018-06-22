#include "stdafx.h"
#include "ComponentTransform.h"
#include "IObject.h"

Component::Component(IObject* pOwner)
    : m_pOwner(pOwner)
{
}

Component::~Component()
{
}

IObject* Component::GetOwner() const
{
    return m_pOwner;
}

Transform* Component::GetOwnerTransform() const
{
	return m_pOwner->GetTransform();
}

Transform::Transform(IObject* pOwner)
	: Component(pOwner)
	, m_Position(0.0f, 0.0f, 0.0f)
	, m_Rotation(0.0f, 0.0f, 0.0f, 0.0f)
	, m_Scale(1.0f, 1.0f, 1.0f)
{
    D3DXMatrixIdentity(&m_Transformation);
}

Transform::~Transform()
{
}

void Transform::SetTM(const D3DXMATRIX& transform)
{
    m_Transformation = transform;
}

const D3DXMATRIX& Transform::GetTM()
{
    D3DXMATRIX t;
    D3DXMatrixTranslation(&t, m_Position.x, m_Position.y, m_Position.z);
    m_Transformation = t;
    return m_Transformation;
}

void Transform::SetPosition(const D3DXVECTOR3& pos)
{
    m_Position = pos;
}

const D3DXVECTOR3 & Transform::GetPosition() const
{
    return m_Position;
}
