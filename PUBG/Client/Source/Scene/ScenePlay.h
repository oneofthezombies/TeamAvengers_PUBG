#pragma once
#include "IScene.h"
#include "Character.h"

class ScenePlay : public IScene
{
public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};