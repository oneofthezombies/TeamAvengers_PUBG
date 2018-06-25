#include "stdafx.h"
#include "ScenePlay.h"
#include "Bandage.h"

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
    //for (int i = 0; i < static_cast<int>(m_Characters.size()); ++i)
        AddObject(new Character(0));
        //AddObject(new Character(1));


    AddObject(new Bandage);
}

void ScenePlay::OnUpdate()
{

}
