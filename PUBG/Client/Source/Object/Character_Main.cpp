#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "CharacterPart.h"
#include "Bullet.h"
#include "Light.h"
#include "AnimationState.h"
#include "Item.h"
#include "ResourceInfo.h"
#include "ComponentTransform.h"

#include "Interpolation.h"

const D3DXQUATERNION Character::OFFSET_ROTATION = 
    D3DXQUATERNION(0.0f, 1.0f, 0.0f, 0.0f);

const float Character::RADIUS = 30.0f;

Character::Character(const int index)
    : IObject(TAG_OBJECT::Character)

    // id
    , m_index(index)
    , m_cellIndex(0)
    , m_health(100.0f)
    , m_isDead(false)

    , m_mouseInput()
    , m_rootTransform(1.0f)
    , m_waistRotation(0.5f/*, 1.0f*/)
    , m_headRotation(1.0f/*, 0.1f*/)

    , m_framePtr()
    , m_info()
    , m_savedInput()
    , m_currentStayKey()
    , m_totalInventory()
    , m_inGameUI()
    , m_attacking(Attacking::Unarmed)
    , m_stance(Stance::Stand)
    , m_moving(Moving::Run)
    , m_direction(Direction::Front)
    , m_upperAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_lowerAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_isFire(false)
    , m_hasChangingState(false)
    , m_isNeedRifleAnim(false)
    , m_isTransitioning(false)

    , pAnimation(nullptr)

    , m_otherHitPart(0)
{
    m_totalInventory.pCharacter = this;
    if (isMine())
    {
        m_totalInventory.Init();
        m_inGameUI.Init();
    }

    const float factor(static_cast<float>(m_index + 1) * 200.0f);

    Transform* pTransform = GetTransform();
    pTransform->SetPosition(D3DXVECTOR3(factor, 200.0f, factor));
    pTransform->SetRotation(OFFSET_ROTATION);

    //putting character into TotalCellSpace
    IScene* CS = CurrentScene()();
    m_cellIndex = CS->GetCellIndex(pTransform->GetPosition());                   //캐릭터의 pos에 따라 알맞은 area에 넣어주기
    CS->InsertObjIntoTotalCellSpace(TAG_OBJECT::Character, m_cellIndex, this);   //Object 를 TotalCellSpace(Area)에 넣기
    CS->m_NearArea.CreateNearArea(m_cellIndex);                                          //Near Area 계산

    pAnimation = new CharacterAnimation(m_index);
    AddChild(pAnimation);
    setAnimation(
        CharacterAnimation::BodyPart::BOTH, 
        TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);
    
    setFramePtr();

    // set boundingShapes
    m_characterParts.resize(static_cast<std::size_t>(TAG_COLLIDER_CHARACTER_PART::COUNT));
    AddPart(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::Head, this));

    for (auto pPart : m_characterParts)
        m_boundingBoxes.emplace_back(pPart->GetBoundingBoxes().front());

    const std::vector<BoundingSphere>& bss = pAnimation->GetBoundingSpheres();
    m_boundingSphere = bss.front();
    m_boundingSphere.position = pTransform->GetPosition();
    // end boundingShapes

    subscribeCollisionEvent();

    if (isMine())
    {
        setInfo();
        Camera()()->SetTarget(&m_info);
        m_rotationForCamera = Vector3::ZERO;
    }

    pOtherHitPositionMesh = Resource()()->GetBoundingSphereMesh();

    m_bBox = BoundingBox::Create(D3DXVECTOR3(-20.0f, 0.0f, -20.0f), D3DXVECTOR3(20.0f, 170.0f, 20.0f));
    //m_bSphereSlidingCollision = BoundingSphere::Create(pTransform->GetPosition(), 50.0f);
}

Character::~Character()
{
    for (auto p : m_characterParts)
    {
        SAFE_DELETE(p);
    }

    if (isMine())
    {
        m_totalInventory.Destroy();
        m_inGameUI.Destroy();
    }
}

void Character::OnUpdate()
{
    const float receivedHealth = Communication()()->m_roomInfo.playerInfos[m_index].health;
    if (receivedHealth < m_health)
        m_health = receivedHealth;

    m_isDead = Communication()()->m_roomInfo.playerInfos[m_index].isDead;

    updateMine();
    updateOther();

    // update
    GetTransform()->Update();      // set characters world
    pAnimation->UpdateAnimation(); // set characters local
    updateBone();                  // modified characters local
    pAnimation->UpdateModel();     // set characters model

    // 캐릭터와 장비 애니메이션 씽크
    syncAnimation();

    // set item animation, item model here
    updateTotalInventory();

    // bounding sphere move to character position
    m_boundingSphere.position = GetTransform()->GetPosition();

    //m_bSphereSlidingCollision.position = GetTransform()->GetPosition();
    m_bBox.position = GetTransform()->GetPosition();
    m_bBox.rotation = GetTransform()->GetRotation();

    for (auto pPart : m_characterParts)
        pPart->Update();

    if (IsFire())
        RifleShooting();
        
    Shader()()->AddShadowSource(
        GetTransform()->GetTransformationMatrix(), 
        pAnimation->GetSkinnedMesh());

    // communication
    communicate();
}

void Character::OnRender()
{
    // render
    if (CurrentCamera()()->IsObjectInsideFrustum(
        m_boundingSphere.center + m_boundingSphere.position, 
        m_boundingSphere.radius))
    {
        pAnimation->Render(

            /* 여기 월드 인자는 레거시임. 밑 셋매트릭스에 변화를 적용시켜야 함 */
            /*m_framePtr.pWaist->CombinedTransformationMatrix
            **/ GetTransform()->GetTransformationMatrix(),

            [this](LPD3DXEFFECT pEffect)
        {
            pEffect->SetMatrix(
                Shader::World,
                &GetTransform()->GetTransformationMatrix());
        });

        renderTotalInventory();
    }

    // render collision shapes
    for (auto pPart : m_characterParts)
        pPart->Render();

    //m_bSphereSlidingCollision.RenderRed();
    m_bBox.RenderRed();
    m_boundingSphere.Render();
    // end render collision shapes

    // render hited box
    auto& b = m_otherHitBox;
    D3DXMATRIX e, c, r, p, m;
    D3DXMatrixScaling(&e, b.extent.x, b.extent.y, b.extent.z);
    D3DXMatrixTranslation(&c, b.center.x, b.center.y, b.center.z);
    D3DXMatrixRotationQuaternion(&r, &b.rotation);
    D3DXMatrixTranslation(&p, b.position.x, b.position.y, b.position.z);
    m = e * c * r * p;
    const auto& vertices = Resource()()->GetBoundingBoxVertices();
    const auto& indices = Resource()()->GetBoundingBoxIndices();
    Shader::Draw(Resource()()->GetEffect("./Resource/", "Color.fx"), nullptr,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);
        D3DXCOLOR magenta(1.0f, 0.0f, 1.0f, 1.0f);
        pEffect->SetValue("Color", &magenta, sizeof magenta);
    }, [&vertices, &indices]()
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_LINELIST,
            0,
            vertices.size(),
            indices.size() / 2,
            indices.data(),
            D3DFMT_INDEX16,
            vertices.data(),
            sizeof vertices.front());
    });
    // end render hited box
}

void Character::updateMine()
{
    if (!isMine()) return;

    //Dead logic
    if (m_isDead) return;

    //m_health 가 0이 되면 죽는 애니메이션, server communication;
    checkDead();

    const float    dt = Time()()->GetDeltaTime();
    Transform* tm = GetTransform();

    //INPUT CONTROL // m_currentStayKey , m_currentOnceKey 으로 사용
    handleInput(&m_currentStayKey);
    handleInput(&m_currentOnceKey);
    if (!m_totalInventory.IsOpened())
    {
        handleMouse(dt, &m_mouseInput);
    }

    //m_currentState를 저장해 놓고 //dest pos 로 계 산
    State destState;
    destState.position = tm->GetPosition();
    destState.rotation = tm->GetRotation();

    //*********************************

    movementControl(&destState);

    ////////////충돌 체크 Area/////////////////////
    //Terrain과의 충돌체크
    //terrainFeaturesCollisionInteraction(&destState);
    terrainFeaturesCollisionInteraction2(&destState);
    //Item 과의 충돌체크
    itemSphereCollisionInteraction();   //<<이곳 안에 m_currentOnceKey._F = false 하는 로직을 넣어놓았다(나중에 문제 생길 수 있을 것 같다)
    ////////////충돌 체크 Area/////////////////////

    getRight();

    setStance();
    setAttacking();
    setReload();

    if (!m_totalInventory.IsOpened())
    {
        setPunch();
    }

    setInteraction();
    setJump();
    animationControl();
    
    // TODO : 앉아있을 때 점프(스페이스) -> 일어섬
    if (m_savedInput != m_currentStayKey)
    {
        if (m_lowerAnimState == TAG_ANIM_CHARACTER::COUNT)
        {
            //애니메이션이 없습니다.
        }
        else
        {
            CharacterAnimation::BodyPart bodyPart;
            if (m_hasChangingState)
            {
                bodyPart = CharacterAnimation::BodyPart::LOWER;
            }
            else
            {
                bodyPart = CharacterAnimation::BodyPart::BOTH;
            }

            setAnimation(
                bodyPart,
                m_lowerAnimState, true, 0.3f, 0.0f, 0.0f);

            m_savedInput = m_currentStayKey;
        }
    }
    
    D3DXVECTOR3 pos = tm->GetPosition();
    D3DXQUATERNION rot = tm->GetRotation();

    characterRotation(&m_mouseInput); //마우스 위아래 -> 케릭터 머리와 팔 위 아래로 움직임
    cameraCharacterRotation(dt, &rot, &m_mouseInput);//케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    applyTarget_Y_Position(&pos); //apply height and control jumping
    
    // shoot!
    m_isFire = false;
    m_totalInventory.m_bulletFireCoolDown -= dt;
    if (!m_totalInventory.isOpened && m_totalInventory.m_pHand != nullptr)      //인벤토리 열려있을때 금지
    {
        if (m_totalInventory.m_bulletFireCoolDown <= 0.f) 
            m_totalInventory.m_bulletFireCoolDown = 0.f;

        if (m_attacking == Attacking::Rifle && 
            m_currentOnceKey._LButton && 
            !m_currentStayKey._LAlt || 

            (m_attacking == Attacking::Rifle && 
                m_totalInventory.m_pHand->GetAuto() && 
                TAG_RES_STATIC::QBZ == m_totalInventory.m_pHand->GetTagResStatic() && 
                m_currentStayKey._LButton && 
                !m_currentStayKey._LAlt))
        {
            if (m_totalInventory.m_bulletFireCoolDown <= 0.f &&  m_totalInventory.m_pHand->GetNumBullet() > 0)
            {
                if (m_hasChangingState == false) //장전 중일 때는 쏘지못하게
                    m_isFire = true;

            }
        }
    }
    if (m_backAction.Ing)
    {
        backActionFrame();
    }

    tm->SetPosition(pos);
    //if(m_totalInventory.)
    tm->SetRotation(rot);

    //인벤토리 UI 활성화
    if (m_currentOnceKey._Tab)
    {
        if (!m_totalInventory.isOpened)
        {
            m_totalInventory.Open();
        }
        else
        {
            m_totalInventory.Close();
        }
    }
    
    m_totalInventory.Update();

    if (m_currentOnceKey._B && m_totalInventory.m_pHand !=NULL)
    {
        m_totalInventory.m_pHand->ChangeAuto();
    }

    //이 아래 sound는 어디에 쓰이는 것인가요?? 찬응에게 답해 주세요
    Sound()()->Listen(pos, getForward());

    if (Input()()->IsOnceKeyDown(VK_UP))
    {
        BulletPool()()->Fire(Communication()()->m_myInfo,pos + getForward() * 100.0f, getBackward(), 0.1f, 1.0f, TAG_RES_STATIC::QBZ);
    }

    ForDebug();
    Debug << "current        position : " << pos << "\n\n"
        << "current upper animation : "
        << pAnimation->GetUpperAnimationName() << '\n'
        << "current lower animation : "
        << pAnimation->GetLowerAnimationName() << "\n\n";

    //여기
    //for InGameUI
    m_inGameUI.Update(m_totalInventory);

    Communication()()->SendPositionAndRotation(pos, rot);
    Communication()()->SendHeadAngle(m_headRotation.m_angle);
}

void Character::updateOther()
{
    if (isMine()) return;

    auto pInput = Input()();
    auto pCom   = Communication()();
    auto pTr    = GetTransform();
    auto pTime  = Time()();

    GameInfo::PlayerInfo& pi = pCom->m_roomInfo.playerInfos[m_index];

    D3DXVECTOR3 pos;
    D3DXQUATERNION rot;

    D3DXVec3Lerp(&pos, &pTr->GetPosition(), &pi.position, 1.0f);
    D3DXQuaternionSlerp(&rot, &pTr->GetRotation(), &pi.rotation, 1.0f);

    pTr->SetPosition(pos);
    pTr->SetRotation(rot);

    D3DXVECTOR2 headAngle;
    D3DXVec2Lerp(
        &headAngle, 
        &D3DXVECTOR2(m_headRotation.m_angle, 0.0f), 
        &D3DXVECTOR2(pi.headAngle, 0.0f), 1.0f);
    m_headRotation.m_angle = headAngle.x;

    const auto upperAnim = static_cast<TAG_ANIM_CHARACTER>(pi.upperAnimState);
    const auto lowerAnim = static_cast<TAG_ANIM_CHARACTER>(pi.lowerAnimState);
    if (upperAnim != m_upperAnimState)
    {
        m_upperAnimState = upperAnim;
        setAnimation(CharacterAnimation::BodyPart::UPPER, upperAnim, true);
    }

    if (lowerAnim != m_lowerAnimState)
    {
        m_lowerAnimState = lowerAnim;
        setAnimation(CharacterAnimation::BodyPart::LOWER, lowerAnim, true);
    }
}