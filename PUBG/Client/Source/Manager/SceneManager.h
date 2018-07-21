#pragma once
#include "Singleton.h"
#include "TagClientOnly.h"

#define g_pScene        Scene       ()()
#define g_pCurrentScene CurrentScene()()

class IScene;
class Character;

class SceneManager : public Singleton<SceneManager>
{
private:
	unordered_map<TAG_SCENE, IScene*> m_scenes;
	IScene*                           pCurrentScene;
    std::vector<Character*>           m_characters;
    Character* pPlayer;
    std::vector<Character*> others;

             SceneManager();
	virtual ~SceneManager();

public:
	void Init();
	void Destroy();
	void Update();
	void Render();

	void    SetCurrentScene(const TAG_SCENE tag);
	IScene* GetCurrentScene() const;

    void SetupCharacters();
    Character* GetPlayer();
    const std::vector<Character*>& GetOthers();
    const std::vector<Character*>& GetCharacters() const;

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