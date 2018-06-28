#include "stdafx.h"
#include "ScenePlay.h"
#include "DirectionalLight.h"

ScenePlay::ScenePlay()
    : IScene()
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::OnInit()
{
    SetDirectionalLight(new DirectionalLight);

    Communication()()->m_MyInfo.m_ID = 0;
    //for (int i = 0; i < Character::NUM_PLAYER; ++i)
    //    AddObject(new Character(i));
    AddObject(new Character(0));

    LoadObjectsFromFile("./Resource/save.txt");
}

void ScenePlay::OnUpdate()
{

}
