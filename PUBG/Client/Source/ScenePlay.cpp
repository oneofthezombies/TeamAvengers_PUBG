#include "stdafx.h"
#include "ScenePlay.h"
#include "Character.h"

ScenePlay::ScenePlay()
    : IScene()
    , m_Characters()
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::OnInit()
{
    for (int i = 0; i < m_Characters.size(); ++i)
        AddObject(new Character(i));
}

void ScenePlay::OnUpdate()
{

}
