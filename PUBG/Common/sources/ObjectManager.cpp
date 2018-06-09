#include "stdafx.h"
#include "ObjectManager.h"
#include "Object.h"

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

void ObjectManager::AddObject(Object* pObj)
{
	m_objects.emplace(pObj);
}

void ObjectManager::RemoveObject(Object* pObj)
{
	auto search = m_objects.find(pObj);
	if (search == m_objects.end())
		return;

	m_objects.erase(search);
}

Object* ObjectManager::FindByTag(const OBJECT_TAG tag)
{
	Object* p = nullptr;
	for (auto& o : m_objects)
	{
		if (o->)
	}
}

vector<ObjectPtr> ObjectManager::FindObjectsByTag(const OBJECT_TAG tag)
{
	return vector<ObjectPtr>();
}

void ObjectManager::AddToTagList(WORD _tag, IDisplayObject * _pObj)
{
	m_tagList[_tag].push_back(_pObj);
}

void ObjectManager::RemoveFromTagList(WORD _tag, IDisplayObject * _pObj)
{
	m_tagList[_tag].remove(_pObj);
}

IDisplayObject* ObjectManager::FindObjectByTag(WORD _tag)
{
	if (m_tagList[_tag].empty() == true) 
		return NULL;

	return m_tagList[_tag].front();
}

list<IDisplayObject*> ObjectManager::FindObjectsByTag(WORD _tag)
{
	return m_tagList[_tag];
}
