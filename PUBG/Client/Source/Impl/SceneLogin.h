#pragma once
#include "IScene.h"

class SceneLogin : public IScene
{
public:
    SceneLogin();
    virtual ~SceneLogin();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
    virtual void OnRender() override;
};