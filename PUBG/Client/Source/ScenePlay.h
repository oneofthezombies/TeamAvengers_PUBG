#pragma once
#include "IScene.h"

class Cube;

class ScenePlay : public IScene
{
private:
    array<Cube*, 2> m_cubes;

public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
    virtual void OnRender() override;
};