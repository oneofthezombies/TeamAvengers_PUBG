#pragma once
#include "IScene.h"
#include "Character.h"

class ScenePlay : public IScene
{
private:
    Character* pPlayer;
    std::vector<Character*> others;
    std::vector<Character*> characters;

    void setAloneMode();
    void setWithOthersMode();

public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void OnInit() override;
    virtual void OnUpdate() override;

    const std::vector<Character*> GetOthers() const;
    int GetSurvivors() const;
    Character* GetPlayer() const;
    const std::vector<Character*>& GetCharacters() const;
};