#include "stdafx.h"
#include "ObjectManager.h"


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::AddObject(BaseObject* pObj)
{
	m_setObject.insert(pObj);
}

void ObjectManager::RemoveObject(BaseObject* pObj)
{
	m_setObject.erase(pObj);
}

void ObjectManager::Destroy()
{
    if (!m_setObject.empty())
        MessageBox(NULL, TEXT("������ ��ü �� �������� ���� ��ü�� �ֽ��ϴ�."), 0, 0);
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
