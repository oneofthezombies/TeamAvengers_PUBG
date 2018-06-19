#pragma once
#include "Singleton.h"
#include "TagObject.h"

#define g_pObjectManager ObjectManager::GetInstance()

class IObject;

class ObjectManager : public Singleton<ObjectManager>
{
private:
	unordered_set<IObject*> m_objects;

    ObjectManager();
    virtual ~ObjectManager();

public:
	void Destroy();

	void AddObject(IObject* p);
	void RemoveObject(IObject* p);

	IObject* FindByTag(const TAG_OBJECT tag);
	vector<IObject*> FindObjectsByTag(const TAG_OBJECT tag);

    friend Singleton<ObjectManager>;
};

