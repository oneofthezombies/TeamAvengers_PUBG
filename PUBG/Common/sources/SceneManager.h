#pragma once
#include "Singleton.h"

#define g_pSceneManager SceneManager::GetInstance()
#define g_pCurrentScene SceneManager::GetInstance()->m_pCurrScene

class IScene;

class SceneManager : public Singleton<SceneManager>
{
private:
	SceneManager();
	virtual ~SceneManager();

public:
	vector<IScene*> m_vecScene;

	IScene* m_pCurrScene;

	void Init();
	void Destroy();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetCurrentScene(size_t index);

	friend Singleton<SceneManager>;
};

