#pragma once
#include "IObject.h"
#include "TagClientOnly.h"
#include "TagAnimation.h"

class SkinnedMeshController;
class CharacterPart;
class Item;

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
    static const int            NUM_PLAYER = 4;
    static const D3DXQUATERNION OFFSET_ROTATION;

private:
    int                m_index;
    TAG_ANIM_CHARACTER m_animState;

    RootTransform m_rootTransform;
    WaistRotation m_waistRotation;
    FramePtr      m_framePtr;

    // for root position
    LPD3DXMESH m_pSphereMesh;

    SkinnedMeshController* pSkinnedMeshController;
    CharacterPart*         m_pRootCharacterPart;

    D3DXMATRIX m_prevRootModel;
    D3DXVECTOR3 m_rotForCameraTP;
    TargetTransform* pTargetTransform;

    //for inventory
    //map<TAG_ITEM_CATEGORY, vector<Item*>> m_mapInventory;
    //¿©±âºÎÅÍÇØ¾ßÇÔ
    map<TAG_RES_STATIC, vector<Item*>> m_mapInventory; //Åº¾à, ¼Ò¸ðÇ°, ÃÑ±âºÎÂø¹°¿ë
    map<TAG_RES_STATIC, vector<Item*>> m_mapEquip;     //Çï¸ä, °¡¹æ, ¹æÅºÁ¶³¢¿ë
    map<TAG_RES_STATIC, vector<Item*>> m_mapWeapon;    //¹«±â¿ë
    float m_capacity;

private:
    void setFramePtr();
    void subscribeCollisionEvent();

    void updateMine();
    void updateOther();
    void updateBone();
    void updateDependency();
    void communicate();

    void rotateWaist(const float quantity);

    bool isMine() const;

    void setAnimation(
        const TAG_ANIM_CHARACTER tag, const bool isBlend, 
        const float currentWeight = 1.0f, const float nextWeight = 0.0f,
        const float blendTime = 0.3f);

    D3DXVECTOR3 getForward();
    D3DXVECTOR3 getRight();

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

    //for inventory
    void PutItemInInventory(Item* item);
    void ShowInventory();
    string ForDebugGetItemCategory(TAG_ITEM_CATEGORY category);
};
