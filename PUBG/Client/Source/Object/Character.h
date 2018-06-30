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
    //여기부터해야함
    map<TAG_RES_STATIC, vector<Item*>> m_mapInventory; //탄약, 소모품, 총기부착물용
    map<TAG_RES_STATIC, vector<Item*>> m_mapEquip;     //헬멧, 가방, 방탄조끼용
    map<TAG_RES_STATIC, vector<Item*>> m_mapWeapon;    //무기용
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

    //이미 인벤토리에 있는 경우, 기존 개수와 합치는 함수
    void CreateOrMergeItem(map<TAG_RES_STATIC, vector<Item*>>* map, Item* item);

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
    void PutItemInTotalInventory(Item* item);
    void ShowTotalInventory();
    string ForDebugGetItemCategory(TAG_ITEM_CATEGORY category);
};
