#pragma once
#include "IScene.h"
#include "Character.h"

class DeathDropBox;
class UIImage;

class ScenePlay : public IScene
{
private:
    std::vector<DeathDropBox*> deathDropBoxes;
    UIObject* m_layer;
    const float m_coolTime;
    float m_coolDown;
    UIImage* pSplash;

    std::set<IObject*> firstGroup;
    std::set<IObject*> secondGroup;

private:
    void setAloneMode();
    void setWithOthersMode();
    void setEmptyBullets();

public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void Render() override;
    virtual void AddObject(IObject* p) override;

    virtual void OnInit() override;
    virtual void OnUpdate() override;

    const std::vector<Character*>& GetOthers() const;
    int GetSurvivors() const;
    Character* GetPlayer() const;
    const std::vector<Character*>& GetCharacters() const;
    DeathDropBox* GetDeathDropBox(const std::size_t index);
    
    UIObject* GetLayer(int layerIndex) const;
    void AddCharacters();
};