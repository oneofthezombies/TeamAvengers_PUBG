#pragma once
#include "IObject.h"
#include "TagAnimCharacter.h"

class MeshFilter;
class Animator;
class SkinnedMeshRenderer;
class CharacterCollisionListener;

class Character : public IObject
{
public:
    struct WaistRotation
    {
        const float LIMIT_OF_ANGLE;
        const float QUANTITY_FACTOR;
        float       m_Angle;

        WaistRotation(const float limit, const float factor);
    };

    struct RootTransform
    {
        const float MOVE_SPEED;
        D3DXVECTOR3 m_Direction;

        RootTransform(const float moveSpeed);
    };

private:
    int m_Index;
    RootTransform m_RootTransform;
    WaistRotation m_WaistRotation;
    LPD3DXMESH    m_pSphereMesh;

    TAG_ANIM_CHARACTER m_AnimState;

    MeshFilter*                 pMeshFilter;
    Animator*                   pAnimator;
    SkinnedMeshRenderer*        pSkinnedMeshRenderer;
    CharacterCollisionListener* pCollisionListener;

    Frame* pWaist;
    Frame* pRoot;

private:
    bool IsMine() const;
    void UpdateTransform();
    void RotateWaist(const float quantity);

public:
    Character(const int index);
    virtual ~Character();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};
