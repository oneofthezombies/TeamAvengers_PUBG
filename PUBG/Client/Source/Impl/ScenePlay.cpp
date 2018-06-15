#include "stdafx.h"
#include "ScenePlay.h"
#include "Cube.h"

ScenePlay::ScenePlay()
    : IScene()
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::OnInit()
{
    for (auto& c : m_cubes)
    {
        c = new Cube;
        AddObject(c);
    }
}

void ScenePlay::OnUpdate()
{
    const auto com = g_pCommunicator;

    const auto id = com->info.myID;
    if (id >= 0)
        m_cubes[id]->SetIsMine(true);

    for (auto& i : com->info.ID)
        if (i > -1)
            m_cubes[i]->SetID(i);

    for (int i = 0; i < com->info.position.size(); ++i)
        m_cubes[i]->SetPosition(com->info.position[i]);
}

void ScenePlay::OnRender()
{
}
