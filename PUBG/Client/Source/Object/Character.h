#pragma once
#include "IObject.h"
#include "TagClientOnly.h"
#include "TagAnimation.h"
#include "AnimationState.h"

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

    struct TotalInventory
    {
        Item* m_hand; //손에 든 무기

        static const float DEFAULT_CAPACITY;

        map<TAG_RES_STATIC, vector<Item*>> m_mapInventory; //탄약, 소모품, 총기부착물용

        //헬멧, 가방, 방탄조끼용
        Item* m_equipArmor;
        Item* m_equipBack;
        Item* m_equipHead;

        //무기용
        Item* m_weaponPrimary;
        Item* m_weaponSecondary;
        //Weapon* m_weaponPistol;
        //Weapon* m_weaponMelee;
        //Weapon* m_weaponThrowable;
        
        float m_bulletFireCoolDown; //총알 발사는 지정된 쿨타임 시간이 지나야 다시 발사가능
        float m_capacity;
        int   m_numReload;

         TotalInventory();
        ~TotalInventory();
    };

    struct Info
    {
        Transform*   pTransform;
        D3DXVECTOR3* pRotationForCamera;
        Frame*       pFPP;
        Frame*       pTPP;

        Info();
    };

    struct IsPressing
    {
        bool _LAlt    ;
        bool _LCtrl   ;
        bool _LShift  ;
        bool _W       ;
        bool _S       ;
        bool _A       ;
        bool _D       ;

        IsPressing();
        bool operator==(const IsPressing& other) const;
        bool operator!=(const IsPressing& other) const;
    };

    struct IsPressed
    {
        bool _Z;
        bool _X;
        bool _C;
        bool _R;
        bool _Space;
        bool _Num1; 
        bool _Num2; 
        bool _Num3; 
        bool _Num4; 
        bool _Num5; 
        bool _LButton;
        bool _RButton;

        IsPressed();
    };

    struct FramePtr
    {
        Frame* pWaist;
        Frame* pRoot;
        Frame* pHandGun;
        Frame* pTPP;
        Frame* pFPP;
        Frame* pSlotPrimary;
        Frame* pSlotSecondary;
        Frame* pSlotMelee;
        Frame* pSlotThrowable;

        FramePtr();
    };

    struct IsJumping
    {
        bool			isJumping;
        float			jumpPower;
        float			gravity;
        float			currGravity;
        float			maxStepHeight;

        IsJumping();
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
    D3DXVECTOR3 m_rotationForCamera;

    Info m_info;
    
    //for inventory
    TotalInventory m_totalInventory;
    

    IsPressing m_savedInput;
    IsPressing m_currentStayKey;
    IsPressed  m_currentOnceKey;

    Stance    m_stance;
    Attacking m_attacking;

    IsJumping m_Jump;

private:
    void setFramePtr();
    void subscribeCollisionEvent();

    void handleInput(IsPressing* OutIsPressing);
    void handleInput(IsPressed* OutIsPressed);

    void setAttacking();
    void setStance();
    void setReload();
    
    void cameraCharacterRotation(const float dt, D3DXQUATERNION* OutRotation);
    void animationMovementControl(D3DXVECTOR3* OutPosition, TAG_ANIM_CHARACTER* OutTag);
    void applyTarget_Y_Position(OUT D3DXVECTOR3* pOut);
    void rifleShooting();

    void updateMine();
    void updateOther();
    void updateBone();
    void updateDependency();
    void communicate();

    void rotateWaist(const float quantity);

    bool isMine() const;

    void setAnimation(
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend = true,
        const float blendTime = 0.3f,
        const float nextWeight = 0.0f);

    void setAnimation(
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const std::function<void()>& finishEvent);

    bool hasFinishEvent() const;
    
    void setInfo();

    D3DXVECTOR3 getUp();
    //D3DXVECTOR3 getDown();
    D3DXVECTOR3 getForward();
    D3DXVECTOR3 getBackward();
    D3DXVECTOR3 getLeft();
    D3DXVECTOR3 getRight();
    
    D3DXVECTOR3 getForwardLeft();
    D3DXVECTOR3 getForwardRight();
    D3DXVECTOR3 getBackwardLeft();
    D3DXVECTOR3 getBackwardRight();
    
    //for inventory
    //이미 인벤토리에 있는 경우, 기존 개수와 합치는 함수
    void createOrMergeItem(map<TAG_RES_STATIC, vector<Item*>>* map, Item* item);
    //이미 아이템이 있는 경우, 그 아이템을 바닥에 떨구고 새아이템을 착용한다
    void checkOriginItem(Item** originItem, Item* newItem);

    // TODO : impl
    void equipItem() {}
    
    void updateTotalInventory();
    void renderTotalInventory();

    void onKar98kReloadEnd();
    void onKar98kReload();

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

    //for Debug
    string ForDebugGetItemCategory(TAG_ITEM_CATEGORY category);
    string ForDebugGetAttacking(Attacking attcking);
    string ForDebugGetStance(Stance stance);
};
