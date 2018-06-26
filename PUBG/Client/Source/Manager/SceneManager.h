#pragma once
#include "Singleton.h"
#include "TagClientOnly.h"

class IScene;

class SceneManager : public Singleton<SceneManager>
{
private:
	unordered_map<TAG_SCENE, IScene*> m_scenes;
	IScene*                           pCurrentScene;

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

struct Scene
{
    SceneManager* operator()()
    {
        return SceneManager::GetInstance();
    }
};

struct CurrentScene
{
    IScene* operator()()
    {
        return SceneManager::GetInstance()->GetCurrentScene();
    }
};