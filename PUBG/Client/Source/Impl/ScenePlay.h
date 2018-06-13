#pragma once
#include "IScene.h"

class ScenePlay : public IScene
{
public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
    virtual void OnRender() override;
};