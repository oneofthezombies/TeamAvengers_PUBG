#include "stdafx.h"
#include "BaseObject.h"
#include "ComponentBase.h"

BaseObject::BaseObject()
	:m_refCount(1)
{
    g_pObjectManager->AddObject(this);
}

BaseObject::~BaseObject()
{
    for (auto& kv : m_umapComponents)
        SAFE_DELETE(kv.second);

    g_pObjectManager->RemoveObject(this);
	assert(m_refCount <= 0 && "Release 를 이용해서 해제하세요.");
}

void BaseObject::AddRef()
{
	m_refCount++;
}

void BaseObject::Release()
{
	m_refCount--;
	if (m_refCount <= 0)
	{
		delete this;
	}
}
