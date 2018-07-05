#pragma once
#include "IObject.h"
#include "TagClientOnly.h"
#include "TagAnimation.h"
#include "AnimationState.h"

class CharacterAnimation;
class CharacterPart;
class Item;

class Character : public IObject
{




/*****************************************************************************/
/*                                                                           */
/*                              nested structure                             */
/*                                                                           */
/*****************************************************************************/

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
        Item* m_pHand; //�տ� �� ����

        static const float DEFAULT_CAPACITY;

        map<TAG_RES_STATIC, vector<Item*>> m_mapInventory; //ź��, �Ҹ�ǰ, �ѱ��������

        //���, ����, ��ź������
        Item* m_pEquipArmor;
        Item* m_pEquipBack;
        Item* m_pEquipHead;

        //�����
        Item* m_pWeaponPrimary;
        Item* m_pWeaponSecondary;
        //Weapon* m_pWeaponPistol;
        //Weapon* m_pWeaponMelee;
        //Weapon* m_pWeaponThrowable;
        
        float m_bulletFireCoolDown; //�Ѿ� �߻�� ������ ��Ÿ�� �ð��� ������ �ٽ� �߻簡��
        float m_capacity;
        int   m_numReload;

        Item* pTempSaveWeaponForX;

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
        bool _F;
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




/**************************** end nested structure ***************************/
 


 
/*****************************************************************************/
/*                                                                           */
/*                                 constant                                  */
/*                                                                           */
/*****************************************************************************/

public:
    static const D3DXQUATERNION OFFSET_ROTATION;

/******************************* end constant ********************************/




/*****************************************************************************/
/*                                                                           */
/*                              member variable                              */
/*                                                                           */
/*****************************************************************************/

private:

    // id
    int m_index;

    Transform*          pTransform;
    CharacterAnimation* pAnimation;
    CharacterPart*      m_pRootCharacterPart;

    //
    FramePtr      m_framePtr;
    RootTransform m_rootTransform;
    WaistRotation m_waistRotation;

    // collision part


    // for camera
    D3DXMATRIX m_prevRootModel;
    D3DXVECTOR3 m_rotationForCamera;
    Info m_info;

    // for inventory
    TotalInventory m_totalInventory;
    

    // state
    TAG_ANIM_CHARACTER m_upperAnimState;
    TAG_ANIM_CHARACTER m_lowerAnimState;

    IsPressing m_savedInput;
    IsPressing m_currentStayKey;
    IsPressed  m_currentOnceKey;

    Stance    m_stance;
    Attacking m_attacking;

    IsJumping m_Jump;

/**************************** end member variable ****************************/




/*****************************************************************************/
/*                                                                           */
/*                               member method                               */
/*                                                                           */
/*****************************************************************************/

private:
    void setFramePtr();
    void subscribeCollisionEvent();

    void handleInput(IsPressing* OutIsPressing);
    void handleInput(IsPressed* OutIsPressed);
    
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
    //�̹� �κ��丮�� �ִ� ���, ���� ������ ��ġ�� �Լ�
    void createOrMergeItem(map<TAG_RES_STATIC, vector<Item*>>* map, Item* item);
    //�̹� �������� �ִ� ���, �� �������� �ٴڿ� ������ ���������� �����Ѵ�
    void checkOriginItem(Item** originItem, Item* newItem);

    // TODO : impl
    void equipItem() {}
    
    void updateTotalInventory();
    void renderTotalInventory();

    //for Character_Input.cpp
    void setStance();
    void setAttacking();
    void setReload();

    void setRifleOnHand(TAG_RIFLE tagRifle);
    void setRifleOnBody(TAG_RIFLE tagRifle);

    void setStandTo(Stance stance);
    void setCrouchTo(Stance stance);
    void setProneTo(Stance stance);

    void onKar98kReloadEnd();
    void onKar98kReload();


/**************************** end member method ******************************/




/*****************************************************************************/
/*                                                                           */
/*                               public method                               */
/*                                                                           */
/*****************************************************************************/

public:
             Character(const int index);
    virtual ~Character();

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void OnCollisionEnter(Collider* pOffence, Collider* pDefence);
    void OnCollisionStay (Collider* pOffence, Collider* pDefence);
    void OnCollisionExit (Collider* pOffence, Collider* pDefence);

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
    void ForDebug();

    CharacterAnimation* GetCharacterAnimation();

/**************************** end public method ******************************/




};
