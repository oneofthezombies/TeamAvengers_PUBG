#pragma once
#include "IObject.h"
#include "TagClientOnly.h"
#include "TagAnimation.h"

class SkinnedMeshController;
class CharacterPart;
class Item;
class Weapon;

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

    struct TotalInventory
    {
        static const float DEFAULT_CAPACITY;

        map<TAG_RES_STATIC, vector<Item*>> m_mapInventory; //탄약, 소모품, 총기부착물용

        //헬멧, 가방, 방탄조끼용
        Item* m_equipArmor;
        Item* m_equipBack;
        Item* m_equipHead;

        //무기용
        Weapon* m_weaponPrimary;
        Weapon* m_weaponSecondary;
        //Weapon* m_weaponPistol;
        //Weapon* m_weaponMelee;
        //Weapon* m_weaponThrowable;
        float m_capacity;

         TotalInventory();
        ~TotalInventory();
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
    TotalInventory m_totalInventory;

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

    //for inventory
    //이미 인벤토리에 있는 경우, 기존 개수와 합치는 함수
    void createOrMergeItem(map<TAG_RES_STATIC, vector<Item*>>* map, Item* item);
    //이미 아이템이 있는 경우, 그 아이템을 바닥에 떨구고 새아이템을 착용한다
    void checkOriginItem(Item** originItem, Item* newItem);
    void updateTotalInventory();
    void renderTotalInventory();

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
