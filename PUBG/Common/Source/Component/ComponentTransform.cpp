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
	, m_position(0.0f, 0.0f, 0.0f)
	, m_rotation(0.0f, 0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
{
}

Transform::~Transform()
{
}
