#pragma once
#include "Singleton.h"
#include "TagScene.h"

#define g_pSceneManager SceneManager::GetInstance()
#define g_pCurrentScene SceneManager::GetInstance()->GetCurrentScene()

class IScene;

class SceneManager : public Singleton<SceneManager>
{
private:
	unordered_map<TAG_SCENE, IScene*> m_scenes;
	IScene* m_pCurrentScene;

	SceneManager();
	virtual ~SceneManager();

public:
	void Init();
	void Destroy();
	void Update();
	void Render();

	void    SetCurrentScene(const TAG_SCENE tag);
	IScene* GetCurrentScene() const;

	friend Singleton<SceneManager>;
};

