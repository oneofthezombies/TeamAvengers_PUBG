#pragma once
#include "IScene.h"
#include "Character.h"

class ScenePlay : public IScene
{
private:
    void setAloneMode();
    void setWithOthersMode();

public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};