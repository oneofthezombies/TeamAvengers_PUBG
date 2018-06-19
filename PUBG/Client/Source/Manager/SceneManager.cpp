#include "stdafx.h"
#include "SceneManager.h"
#include "IScene.h"
#include "SceneLogin.h"
#include "ScenePlay.h"

SceneManager::SceneManager()
	: Singleton<SceneManager>()
	, m_pCurrentScene(nullptr)
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
	if (m_pCurrentScene) m_pCurrentScene->Update();
}

void SceneManager::Render()
{
	if (m_pCurrentScene) m_pCurrentScene->Render();
}

void SceneManager::SetCurrentScene(const TAG_SCENE tag)
{
	const auto search = m_scenes.find(tag);
	if (search != m_scenes.end())
	{
		auto s = search->second;
		if (s)
		{
			m_pCurrentScene = s;
			m_pCurrentScene->Init();
		}
	}
}

IScene* SceneManager::GetCurrentScene() const
{
	return m_pCurrentScene;
}
