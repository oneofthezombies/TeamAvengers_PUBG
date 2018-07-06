#pragma once
#include "IScene.h"
#include "Character.h"

class ScenePlay : public IScene
{
private:
    Character* pPlayer;
    std::vector<Character*> m_others;

    void setAloneMode();
    void setWithOthersMode();

public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void OnInit() override;
    virtual void OnUpdate() override;

    const std::vector<Character*> GetOthers() const;
};