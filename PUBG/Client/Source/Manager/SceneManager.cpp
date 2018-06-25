#include "stdafx.h"
#include "SceneManager.h"
#include "IScene.h"
#include "SceneLogin.h"
#include "ScenePlay.h"

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
    m_scenes.emplace(TAG_SCENE::LOGIN, new SceneLogin);
    m_scenes.emplace(TAG_SCENE::PLAY, new ScenePlay);

    SetCurrentScene(TAG_SCENE::LOGIN);
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
