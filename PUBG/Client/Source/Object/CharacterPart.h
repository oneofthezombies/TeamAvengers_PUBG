#pragma once
#include "IObject.h"
#include "TagClientOnly.h"

class Character;
class BoxCollider;
class UIText;
class SkinnedMeshController;
struct Frame;

class CharacterPart : public IObject
{
private:
    const TAG_COLLIDER_CHARACTER_PART m_tagColliderCharacterPart;
    vector<Frame*> m_frames;
    vector<UIText*> m_UITexts;

    Character*   pCharacter;
    BoxCollider* pBoxCollider;

    void addFrame(const string& name, SkinnedMeshController* pSkiCon);
    void updateUI();

public:
    CharacterPart(
        const TAG_COLLIDER_CHARACTER_PART tag, Character* pCharacter);
    
    virtual ~CharacterPart();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    TAG_COLLIDER_CHARACTER_PART GetTagColliderCharacterPart() const;
};