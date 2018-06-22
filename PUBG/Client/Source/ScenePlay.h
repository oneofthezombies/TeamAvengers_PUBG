#pragma once
#include "IScene.h"

class Character;

class ScenePlay : public IScene
{
private:
    array<Character*, 4> m_Characters;

public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};