#include "stdafx.h"
#include "IObject.h"
#include "ComponentTransform.h"

IObject::IObject()
    : MemoryAllocator()
    , pTransform(nullptr)
    , pParent(nullptr)

{
	pTransform = AddComponent<Transform>();
}

IObject::~IObject()
{
    for (auto c : m_children)
        SAFE_DELETE(c);

    for (auto c : m_components)
        SAFE_DELETE(c.second);
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

void IObject::SetTagObject(const TAG_OBJECT tag)
{
    m_tagObject = tag;
}

TAG_OBJECT IObject::GetTagObject() const
{
	return m_tagObject;
}

void IObject::SetParent(IObject* pParent)
{
    assert(pParent && "IObject::SetParent(), pointer is null.");
	this->pParent = pParent;
}

void IObject::AddChildren(IObject* pChild)
{
    assert(pChild && "IObject::SetParent(), pointer is null.");
	pChild->SetParent(this);
	m_children.emplace_back(pChild);
}

Transform* IObject::GetTransform()
{
	return pTransform;
}
