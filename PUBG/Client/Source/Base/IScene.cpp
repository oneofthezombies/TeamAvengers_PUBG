#include "stdafx.h"
#include "IScene.h"
#include "IObject.h"

IScene::IScene()
    : MemoryAllocator()
{
}

IScene::~IScene()
{
	for (auto o : m_objects)
        SAFE_DELETE(o);
}

void IScene::Init()
{
    OnInit();
}

void IScene::Update()
{
	for (auto& o : m_objects)
		if (o) o->Update();

	OnUpdate();

	updateToDeleteObjects();
}

void IScene::Render()
{
	for (auto& o : m_objects)
		if (o) o->Render();
}

void IScene::AddObject(IObject* p)
{
    assert(p && "IScene::AddObject(), object is null.");

	m_objects.emplace(p);
}

void IScene::RemoveObject(IObject* p)
{
    assert(p && "IScene::RemoveObject(), object is null.");

	m_objects.erase(p);
}

void IScene::Destroy(IObject* p, const float t)
{
	if (!p) return;

	const auto search = m_toDeleteObjects.find(p);
	if (search != m_toDeleteObjects.end()) return;

	m_toDeleteObjects.emplace(p, t);
}

void IScene::updateToDeleteObjects()
{
	const float dt = Time()()->GetDeltaTime();

	const auto begin = m_toDeleteObjects.begin();
	const auto end = m_toDeleteObjects.end();
	for (auto it = begin; it != end;)
	{
		auto p = it->first;
		auto& t = it->second;

		t -= dt;
		if (t > 0.0f)
		{
			++it;
		}
		else
		{
			RemoveObject(p);
            SAFE_DELETE(p);
			it = m_toDeleteObjects.erase(it);
		}
	}
}
