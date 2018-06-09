#include "stdafx.h"
#include "Object.h"
#include "ComponentTransform.h"
#include "ObjectManager.h"

Object::Object()
{
    g_pObjectManager->AddObject(this);

	m_pTransform = AddComponent<Transform>();
}

Object::~Object()
{
	for (auto& c : m_children)
		SAFE_DELETE(c);

	for (auto& c : m_components)
		SAFE_DELETE(c.second);

    g_pObjectManager->RemoveObject(this);
}

void Object::SetTag(const OBJECT_TAG tag)
{
	m_tag = tag;
}

OBJECT_TAG Object::GetTag() const
{
	return m_tag;
}

void Object::SetParent(Object* pParent)
{
	m_pParent = pParent;
}

void Object::AddChildren(Object* pChild)
{
	pChild->SetParent(this);
	m_children.emplace_back(pChild);
}

Transform* Object::GetTransform()
{
	return m_pTransform;
}
