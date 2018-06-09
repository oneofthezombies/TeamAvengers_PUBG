#include "stdafx.h"
#include "SceneManager.h"
#include "IScene.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
    m_vecScene.push_back(new SceneLobby);
    m_vecScene.push_back(new SceneShotting);
    m_vecScene.push_back(new SceneTest);
	m_vecScene.push_back(new SceneGrid);

	//m_vecScene.push_back(new SceneObjMap());
	//m_vecScene.push_back(new SceneHeightmap());


    SetCurrentScene(SCENE_LOBBY);
    //SetCurrentScene(SCENE_SHOTTING);
    //SetCurrentScene(SCENE_TEST);
	//SetCurrentScene(SCENE_GRID);

	//SetCurrentScene(SCENE_OBJMAP);
	//SetCurrentScene(SCENE_HEIGHTMAP);

}

void SceneManager::Destroy()
{
	for (auto p : m_vecScene)
	{
		SAFE_RELEASE(p);
	}
}

void SceneManager::Update()
{
    //JH

	SAFE_UPDATE(m_pCurrScene);
}

void SceneManager::Render()
{
	SAFE_RENDER(m_pCurrScene);
}

void SceneManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//if (m_pCurrScene)
	//	m_pCurrScene->WndProc(hWnd, message, wParam, lParam);
}

void SceneManager::SetCurrentScene(size_t index)
{
	if (index >= m_vecScene.size()) return;

	IScene* pScene = m_vecScene[index];
	if (pScene)
	{
        m_pCurrScene = pScene;
		pScene->Init();
	}
}
