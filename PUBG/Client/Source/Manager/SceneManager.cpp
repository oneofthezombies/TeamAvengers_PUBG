#include "stdafx.h"
#include "SceneManager.h"
#include "IScene.h"
#include "SceneLogin.h"
#include "SceneLobby.h"
#include "ScenePlay.h"
#include "SceneLoading.h"
#include "SceneCollisionTest.h"
#include "Character.h"

SceneManager::SceneManager()
	: Singleton<SceneManager>()
	, pCurrentScene(nullptr)
    , pPlayer(nullptr)
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

void SceneManager::SetupCharacters()
{
    m_characters.resize(GameInfo::NUM_PLAYERS);
    for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
        m_characters[i] = new Character(i);
}

Character* SceneManager::GetPlayer()
{
    if (!pPlayer)
    {
        const int myID = Communication()()->m_myInfo.ID;
        pPlayer = m_characters[myID];
    }
    return pPlayer;
}

const std::vector<Character*>& SceneManager::GetOthers()
{
    if (others.empty())
    {
        const int myID = Communication()()->m_myInfo.ID;
        for (std::size_t i = 0; i < m_characters.size(); i++)
        {
            if (i == myID) continue;

            others.emplace_back(m_characters[i]);
        }
    }
    return others;
}

const std::vector<Character*>& SceneManager::GetCharacters() const
{
    return m_characters;
}
