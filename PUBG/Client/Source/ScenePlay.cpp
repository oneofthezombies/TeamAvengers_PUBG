#include "stdafx.h"
#include "ScenePlay.h"
#include "Bandage.h"
#include "Kar98k.h"

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
    for (int i = 0; i < static_cast<int>(m_Characters.size()); ++i)
        AddObject(new Character(i, nullptr));

    AddObject(new Bandage(Vector3::RIGHT * -200.0f, Vector3::ONE * 1.0f));
    AddObject(new Bandage(Vector3::RIGHT * 300.0f, Vector3::ONE * 1.0f));
    AddObject(new Kar98k);
}

void ScenePlay::OnUpdate()
{

}
