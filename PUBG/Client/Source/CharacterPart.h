#pragma once
#include "IObject.h"
#include "TagColliderCharacterPart.h"

class Character;
class BoxCollider;
class BoxColliderRenderer;
class MeshFilter;
class UIText;
struct Frame;

class CharacterPart : public IObject
{
private:
    const TAG_COLLIDER_CHARACTER_PART m_Tag;
    vector<Frame*> m_Frames;
    vector<UIText*> m_UITexts;

    Character*           pCharacter;
    BoxCollider*         pBoxCollider;
    BoxColliderRenderer* pBoxColliderRenderer;

    void addFrame(const string& name, MeshFilter* pMeshFilter);
    void updateUI();

public:
    CharacterPart(
        const TAG_COLLIDER_CHARACTER_PART tag, Character* pCharacter);
    
    virtual ~CharacterPart();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    TAG_COLLIDER_CHARACTER_PART GetTagColliderCharacterPart() const;
};