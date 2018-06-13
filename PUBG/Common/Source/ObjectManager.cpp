#include "stdafx.h"
#include "ObjectManager.h"
#include "IObject.h"

ObjectManager::ObjectManager()
	: Singleton<ObjectManager>()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::Destroy()
{
	assert(m_objects.empty() && "ObjectManager::Destroy(), There are objects that have not been deleted.");
}

void ObjectManager::AddObject(IObject* p)
{
	m_objects.emplace(p);
}

void ObjectManager::RemoveObject(IObject* p)
{
	auto search = m_objects.find(p);
	if (search == m_objects.end())
		return;

	m_objects.erase(search);
}

IObject* ObjectManager::FindByTag(const TAG_OBJECT tag)
{
	IObject* p = nullptr;
	for (auto& o : m_objects)
		if (o->GetTag() == tag)
			p = o;

	return p;
}
