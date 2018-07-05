#pragma once
#include "IObject.h"
#include "TagClientOnly.h"

class Character;
class BoxCollider;
class UIText;
class CharacterAnimation;
struct Frame;

class CharacterPart : public IObject
{
private:
    const TAG_COLLIDER_CHARACTER_PART m_tagColliderCharacterPart;
    vector<Frame*> m_frames;
    vector<UIText*> m_UITexts;

    Character*   pCharacter;
    //BoxCollider* pBoxCollider;
    BoundingBox m_boundingBox;

    void addFrame(const string& name, CharacterAnimation* pCharacterAnimation);
    void updateUI();
    void addChild(const TAG_COLLIDER_CHARACTER_PART tag);

public:
    CharacterPart(
        const TAG_COLLIDER_CHARACTER_PART tag, Character* pCharacter);
    
    virtual ~CharacterPart();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    TAG_COLLIDER_CHARACTER_PART GetTagColliderCharacterPart() const;

    BoundingBox* GetBoundingBox();
};