#pragma once
#include "Singleton.h"
#include "ObjectTag.h"

#define g_pObjectManager ObjectManager::GetInstance()

class Object;

class ObjectManager : public Singleton<ObjectManager>
{
private:
	unordered_set<Object*> m_objects;

    ObjectManager();
    virtual ~ObjectManager();

public:
	void Destroy();

	void AddObject(Object* pObj);
	void RemoveObject(Object* pObj);

	Object* FindByTag(const OBJECT_TAG tag);
	vector<Object*> FindObjectsByTag(const OBJECT_TAG tag);

    friend Singleton<ObjectManager>;
};

