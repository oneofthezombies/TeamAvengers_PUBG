#pragma once
#include "IObject.h"
#include "TagClientOnly.h"

class SkinnedMeshController;

class Character : public IObject
{
public:
    struct WaistRotation
    {
        const float LIMIT_OF_ANGLE;
        const float QUANTITY_FACTOR;
        float       m_angle;

        WaistRotation(const float limit, const float factor);
    };

    struct RootTransform
    {
        const float MOVE_SPEED;

        RootTransform(const float moveSpeed);
    };

private:
    struct FramePtr
    {
        Frame* pWaist;
        Frame* pRoot;
        Frame* pHandGun;

        FramePtr();
    };

public:
    static const int NUM_PLAYER = 4;

private:
    int                m_index;
    TAG_ANIM_CHARACTER m_animState;

    RootTransform m_rootTransform;
    WaistRotation m_waistRotation;
    FramePtr      m_framePtr;

    // for root position
    LPD3DXMESH m_pSphereMesh;

    SkinnedMeshController* pSkinnedMeshController;

private:
    void setFramePtr();
    void subscribeCollisionEvent();

    void updateTransform();
    void rotateWaist(const float quantity);

    bool isMine() const;

public:
             Character(const int index);
    virtual ~Character();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void OnCollisionEnter(Collider* pPerpetrator, Collider* pVictim);
    void OnCollisionStay (Collider* pPerpetrator, Collider* pVictim);
    void OnCollisionExit (Collider* pPerpetrator, Collider* pVictim);

    int GetIndex() const;
    TAG_COLLISION GetTagCollisionBody(const int index);
    TAG_COLLISION GetTagCollisionDamage(const int index);
};
