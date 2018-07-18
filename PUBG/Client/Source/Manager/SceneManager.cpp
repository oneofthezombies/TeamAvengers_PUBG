#include "stdafx.h"
#include "SceneManager.h"
#include "IScene.h"
#include "SceneLogin.h"
#include "SceneLobby.h"
#include "ScenePlay.h"
#include "SceneLoading.h"
#include "SceneCollisionTest.h"

SceneManager::SceneManager()
	: Singleton<SceneManager>()
	, pCurrentScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
    m_scenes.emplace(TAG_SCENE::Loading, new SceneLoading);
    m_scenes.emplace(TAG_SCENE::Login,   new SceneLogin);
    m_scenes.emplace(TAG_SCENE::Lobby,   new SceneLobby);
    m_scenes.emplace(TAG_SCENE::Play,    new ScenePlay);

    //m_scenes.emplace(TAG_SCENE::CollisionTest, new SceneCollisionTest);

    SetCurrentScene(TAG_SCENE::Loading);
    //SetCurrentScene(TAG_SCENE::CollisionTest);
}

void SceneManager::Destroy()
{
	for (auto& s : m_scenes)
        SAFE_DELETE(s.second);
}

void SceneManager::Update()
{
	if (pCurrentScene) 
        pCurrentScene->Update();
}

void SceneManager::Render()
{
	if (pCurrentScene) 
        pCurrentScene->Render();
}

void SceneManager::SetCurrentScene(const TAG_SCENE tag)
{
	const auto search = m_scenes.find(tag);
	if (search != m_scenes.end())
	{
		auto s = search->second;
		if (s)
		{
			pCurrentScene = s;
			pCurrentScene->Init();
		}
	}
}

IScene* SceneManager::GetCurrentScene() const
{
	return pCurrentScene;
}
