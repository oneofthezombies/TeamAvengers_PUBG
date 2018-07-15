#pragma once
#include "IObject.h"
#include "TagClientOnly.h"
#include "TagAnimation.h"
#include "AnimationState.h"
#include "CharacterAnimation.h"

class CharacterAnimation;
class CharacterPart;
class Item;
class Area;
class UiButtonListener;

struct FramePtr
{
    Frame* pRoot;
    Frame* pHead;
    Frame* pLeftClavicle;
    Frame* pRightClavicle;
    Frame* pWaist;
    Frame* pHandGun;
    Frame* pTPP;
    Frame* pFPP;
    Frame* pSlotPrimary;
    Frame* pSlotSecondary;
    Frame* pSlotMelee;
    Frame* pSlotThrowable;

    FramePtr();
};

struct MovingFactor
{
    static const float UNARMED_RUN;
    static const float UNARMED_SPRINT;
    static const float UNARMED_WALK;
    static const float RIFLE_RUN;
    static const float RIFLE_SPRINT;
    static const float RIFLE_WALK;
};

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
        //const float QUANTITY_FACTOR;
        float       m_angle;
        bool        m_limit;

        WaistRotation(const float limit/*, const float factor*/);
    };

    struct HeadRotation
    {
        const float LIMIT_OF_ANGLE;
        /*const float QUANTITY_FACTOR;*/
        float       m_angle;

        HeadRotation(const float limit/*, const float factor*/);
    };

    struct RootTransform
    {
        const float MOVE_SPEED;

        RootTransform(const float moveSpeed);
    };

    struct TotalInventory
    {
        static const float DEFAULT_CAPACITY;
        static const int NUM_UI_DROPPED = 12;

        Character* pCharacter;

        UIImage* m_Border;
        UIText*  m_Text;
        std::vector<UIButtonWithItem*> m_uiDroped;
        std::vector<UIButtonWithItem*> m_uiInven;
        UIButtonWithItem* m_pUIPicked;
        UIButtonWithItem* m_pWeapon1;
        UIButtonWithItem* m_pWeapon2;

        Item* m_pHand; //�տ� �� ����
        bool  m_isOnBodyAnimationEnd; //���� �ִϸ��̼��� ��������


        map<TAG_RES_STATIC, vector<Item*>> m_mapInventory; //ź��, �Ҹ�ǰ, �ѱ��������

        //���, ����, ��ź������
        Item* m_pEquipArmor;
        Item* m_pEquipBack;
        Item* m_pEquipHead;

        int m_equipOnNum;

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
        
        //ui
        bool isOpened;
        std::deque<Item*> droppedItems;
        bool m_stateClicked;
        TAG_RIFLE m_handState;

        ////////////�Լ�
        void Init();
        void Destroy();

        void Open();
        void Close();
        void Update();
        void Render();
        bool IsOpened();
        void SetEquipUI();

         TotalInventory();
        ~TotalInventory();
    };

    struct InGameUI
    {
        static const D3DCOLOR RED;
        static const D3DCOLOR WHITE;
        static const D3DCOLOR GRAY;
        static const D3DCOLOR WHITE_ALPHA;
        static const D3DCOLOR BLACK_ALPHA;

        static const D3DXVECTOR3 PRIMARY_WEAPON_POS;
        static const D3DXVECTOR3 SECONDARY_WEAPON_POS;

        static const float EQUIP_START;
        static const float EQUIP_WIDTH;
        static const float EQUIP_HEIGHT;
        static const float EQUIP_GAP;

        UIImage* m_pBackground;

        //Image ===================
        //compass
        UIImage* pCompass;

        //equip
        UIImage* pBagImg;
        UIImage* pHelmetImg;
        UIImage* pVestImg;

        //hp
        UIImage* pHpRedImg;
        UIImage* pHpWhiteImg;

        //weapons
        UIImage* pQBZImg;
        UIImage* pKar98kImg;

        UIImage* pQBZRedImg;
        UIImage* pKar98kRedImg;


        //Text ====================
        //ammo
        UIText* pAmmoReloadText;
        UIText* pAmmoNumText;
        UIText* pFireModeText;

        //survival
        UIText* pSurvivalNumText;

        //kill(������ ���)
        UIText* pKillNumUpText;

        //id, version
        UIText* pIdText;

        //kill(ȭ�� �߾�)
        UIText* pKillNumText;
        UIText* pKillText;

        UIText* pKillNumTextShadow;
        UIText* pKillTextShadow;

        //������ ��� �� �ȳ�����
        UIText* pInfoText;
        UIText* pInfoTextShadow;

        //ų�α�
        UIText* pKillLog1;
        UIText* pKillLog2;

        const float COOL_TIME;
        float m_coolDown;

        InGameUI();
        ~InGameUI();

        void Init();
        void Destroy();
        void Update(const TotalInventory& inven);
        void Render();

        void setTextWithShadow(
            UIText*& pText,
            UIText*& pTextShadow,
            const LPD3DXFONT font,
            const D3DXVECTOR2& size,
            const string& str,
            const D3DCOLOR color,
            UIObject* pParent,
            const D3DXVECTOR3& position);

        void updateInfoTextUI();
        void updateOnHandWeaponUI(const TotalInventory& inven);
        void updateSurvivalNumTextUI();
        
    };

    struct Info
    {
        Transform*   pTransform;
        D3DXVECTOR3* pRotationForCamera;
        Frame*       pHead;
        Frame*       pFPP;
        Frame*       pTPP;
        Frame*       pHandGun;
        Frame*       pGunBolt;

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
        bool _LButton ;
        bool _RButton ;

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
        bool _B;
        bool _F;
        bool _Space;
        bool _Num1; 
        bool _Num2; 
        bool _Num3; 
        bool _Num4; 
        bool _Num5; 
        bool _LButton;
        bool _RButton;
        bool _Tab;

        IsPressed();
    };

    struct MouseInput
    {
        float yaw;
        float pitch;

        MouseInput()
            : yaw(0.0f)
            , pitch(0.0f)
        {
        }
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

    struct State
    {
        D3DXVECTOR3              position;
        D3DXQUATERNION           rotation;
        std::vector<BoundingBox> boundingBoxes;
        bool                     isHeadBump;

        State();
    };

    //�ݵ� ��Ʈ��Ʈ
    struct WaitBackAction
    {
        //D3DXQUATERNION  beforeY;            
        //float           beforeX;
        //D3DXQUATERNION  afterY;
        //float           afterX;
        ////////// ������ ����׿�
        D3DXQUATERNION* Rot;
        float           valX;
        float           valY;
        float           curValX;
        float           curValY;
        bool            Ing = false;
        bool            Up = true;
    };


/**************************** end nested structure ***************************/
 


 
/*****************************************************************************/
/*                                                                           */
/*                                 constant                                  */
/*                                                                           */
/*****************************************************************************/

public:
    static const D3DXQUATERNION OFFSET_ROTATION;
    static const float          RADIUS;

/******************************* end constant ********************************/




/*****************************************************************************/
/*                                                                           */
/*                              member variable                              */
/*                                                                           */
/*****************************************************************************/

private:

    // id
    int                 m_index;
    std::size_t         m_cellIndex;
    float               m_health;
    bool                m_isDead;

    CharacterAnimation*    pAnimation;
    vector<CharacterPart*> m_characterParts;
    
    
    FramePtr      m_framePtr;
    RootTransform m_rootTransform;
    WaistRotation m_waistRotation;
    HeadRotation  m_headRotation;
    

    // for camera
    D3DXMATRIX  m_prevRootModel;
    D3DXVECTOR3 m_rotationForCamera;
    Info        m_info;

    // for inventory
    TotalInventory m_totalInventory;

    // for InGameUI
    InGameUI m_inGameUI;
    
    // state
    TAG_ANIM_CHARACTER m_upperAnimState;
    TAG_ANIM_CHARACTER m_lowerAnimState;
    Stance    m_stance;
    Attacking m_attacking;
    Moving    m_moving;
    Direction m_direction;

    IsPressing m_savedInput;
    IsPressing m_currentStayKey;
    IsPressed  m_currentOnceKey;
    MouseInput m_mouseInput;

    IsJumping m_Jump;

    // for debug
    LPD3DXMESH  pOtherHitPositionMesh;
    std::size_t m_otherHitPart;
    BoundingBox m_otherHitBox;

    bool m_isFire;
    bool m_hasChangingState; //true�� ���� lower�� ��ȭ�Ѵ�
    bool m_isNeedRifleAnim;
    bool m_isTransitioning;  //���� ���� ���� �������� �ʴ´�

    WaitBackAction m_backAction;

    // for character x character collision
    BoundingBox m_bBox;
    //BoundingSphere m_bSphereSlidingCollision;


/**************************** end member variable ****************************/




/*****************************************************************************/
/*                                                                           */
/*                               member method                               */
/*                                                                           */
/*****************************************************************************/

private:
    void setFramePtr();
    void subscribeCollisionEvent();

    void checkDead();
    void handleInput(IsPressing* OutIsPressing);
    void handleInput(IsPressed* OutIsPressed);
    void handleMouse(const float dt, MouseInput* mouseInput);

    void terrainFeaturesCollisionInteraction(OUT State* OutState);
    void terrainFeaturesCollisionInteraction2(OUT State* OutState);
    void itemSphereCollisionInteraction();
    void characterRotation(MouseInput* mouseinput);
    void cameraCharacterRotation(const float dt, D3DXQUATERNION* OutRotation, MouseInput* mouseInput);
    void applyTarget_Y_Position(OUT D3DXVECTOR3* pOut);
    void movementControl(OUT State* OutState);
    void animationControl();

    //�� �ݵ� ����
    void backAction(D3DXQUATERNION* OutRotation ,int virtical = 100 , int horizontal = 50);
    void backActionFrame();

    void updateMine();
    void updateOther();
    void updateBone();
    void communicate();

    void rotateWaist(const float quantity);
    void rotateHead(const float quantity);

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
    void setPunch();
    void setInteraction();
    void setJump();

    void setRifleOnHand(TAG_RIFLE tagRifle);
    void setRifleOnBody(TAG_RIFLE tagRifle);

    void setStandTo(Stance stance);
    void setCrouchTo(Stance stance);
    void setProneTo(Stance stance);

    void onKar98kReloadEnd();
    void onKar98kReload();

    void setAnimation(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend = true,
        const float blendingTime = 0.3f,
        const float nextWeight = 0.0f,
        const float position = 0.0f);

    void setAnimation(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void setAnimation(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float loopEventPeriod,
        const std::function<void()>& loopEvent,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void setEquipAnimation(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend = true,
        const float blendingTime = 0.3f,
        const float nextWeight = 0.0f,
        const float position = 0.0f);

    void setEquipAnimation(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void setEquipAnimation(
        const CharacterAnimation::BodyPart part,
        const TAG_ANIM_CHARACTER tag,
        const bool isBlend,
        const float blendingTime,
        const float nextWeight,
        const float position,
        const float loopEventPeriod,
        const std::function<void()>& loopEvent,
        const float finishEventAgoTime,
        const std::function<void()>& finishEvent);

    void onMouse(
        const UIButtonWithItem::Event event, 
        const UIButtonWithItem::MouseButton button, 
        UIButtonWithItem* pUIButtonWithItem);


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
    float GetCharacterHealth() const;
    bool GetCharacterIsDead() const;

    TAG_COLLISION GetTagCollisionBody(const int index);
    TAG_COLLISION GetTagCollisionDamage(const int index);

    //for inventory
    void PutItemInTotalInventory(Item* item);
    void ShowTotalInventory();

    //for Debug
    string ForDebugGetItemCategory(TAG_ITEM_CATEGORY category);
    string ForDebugGetAttacking(Attacking attcking);
    string ForDebugGetStance(Stance stance);
    string ForDebugGetMoving(Moving moving);
    void   ForDebug();

    CharacterAnimation* GetCharacterAnimation();

    void AddPart(CharacterPart* pPart);
    D3DXVECTOR3 GetWaistPosition();
    bool IsFire() const;
    void RifleShooting();
    D3DXVECTOR3 FindShootingTargetPos();
    void MinusDamage(const float damage);

    //        const BoundingBox&              GetBoundingBox();
    virtual const std::vector<BoundingBox>& GetBoundingBoxes() override;
/**************************** end public method ******************************/




};
