#include "stdafx.h"
#include "ScenePlay.h"
#include "DirectionalLight.h"
#include "TerrainFeature.h"
#include "SkySphere.h"
#include "Grid.h"

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
    //AddObject(new TerrainFeature(TAG_RES_STATIC::SkySphere, Vector3::ZERO, Vector3::ZERO, Vector3::ONE* 0.18f));

    //AddObject(new SkySphere);
    AddObject(new Grid);

    Communication()()->m_MyInfo.m_ID = 0;
    //for (int i = 0; i < Character::NUM_PLAYER; ++i)
    //    AddObject(new Character(i));
    AddObject(new Character(0));

    //LoadObjectsFromFile("./Resource/save.txt");
}

void ScenePlay::OnUpdate()
{

}
