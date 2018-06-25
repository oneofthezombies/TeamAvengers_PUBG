#pragma once
#include "IScene.h"
#include "Character.h"

class ScenePlay : public IScene
{
private:
    array<Character*, Character::NUM_PLAYER> m_Characters;

public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};