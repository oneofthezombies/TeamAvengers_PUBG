#pragma once
#include "IObject.h"
#include "TagColliderCharacterPart.h"

class BoxCollider;
class BoxColliderRenderer;
class MeshFilter;
class UIText;
struct Frame;

class CharacterPart : public IObject
{
private:
    const TAG_COLLIDER_CHARACTER_PART m_tag;
    BoxCollider* pBoxCollider;
    BoxColliderRenderer* pBoxColliderRenderer;
    vector<Frame*> m_Frames;
    vector<UIText*> m_UITexts;

    void AddFrame(const string& name, MeshFilter* pMeshFilter);

public:
    CharacterPart(const TAG_COLLIDER_CHARACTER_PART tag, 
        MeshFilter* pMeshFilter, 
        ICollisionListener* pCollisionListener);
    virtual ~CharacterPart();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};