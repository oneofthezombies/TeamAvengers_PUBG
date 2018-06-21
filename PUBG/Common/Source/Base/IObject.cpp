#include "stdafx.h"
#include "IObject.h"
#include "ComponentTransform.h"
#include "ObjectManager.h"

IObject::IObject()
{
    g_pObjectManager->AddObject(this);

	m_pTransform = AddComponent<Transform>();
}

IObject::~IObject()
{
	for (auto c : m_children)
		SAFE_DELETE(c);

	for (auto& c : m_components)
		SAFE_DELETE(c.second);

    g_pObjectManager->RemoveObject(this);
}

void IObject::Update()
{
	OnUpdate();

	for (auto& c : m_children)
		if (c) c->Update();
}

void IObject::Render()
{
	OnRender();

	for (auto& c : m_children)
		if (c) c->Render();
}

void IObject::SetTag(const TAG_OBJECT tag)
{
	m_tag = tag;
}

TAG_OBJECT IObject::GetTag() const
{
	return m_tag;
}

void IObject::SetParent(IObject* pParent)
{
	m_pParent = pParent;
}

void IObject::AddChildren(IObject* pChild)
{
	pChild->SetParent(this);
	m_children.emplace_back(pChild);
}

Transform* IObject::GetTransform()
{
	return m_pTransform;
}
