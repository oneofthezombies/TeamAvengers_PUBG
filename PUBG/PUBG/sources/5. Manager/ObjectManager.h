#pragma once

#include "SingletonBase.h"

#define g_pObjectManager ObjectManager::GetInstance()

class BaseObject;
class IDisplayObject;

class ObjectManager : public SingletonBase<ObjectManager>
{
private:
	set<BaseObject*> m_setObject;
	map<WORD, list<IDisplayObject*>> m_tagList;

    ObjectManager();
    virtual ~ObjectManager();

public:
	void AddObject(BaseObject* pObj);
	void RemoveObject(BaseObject* pObj);
	void Destroy();

	void AddToTagList(WORD _tag, IDisplayObject* _pObj);
	void RemoveFromTagList(WORD _tag, IDisplayObject* _pObj);
	IDisplayObject* FindObjectByTag(WORD _tag);
	list<IDisplayObject*> FindObjectsByTag(WORD _tag);

    friend SingletonBase<ObjectManager>;
};

