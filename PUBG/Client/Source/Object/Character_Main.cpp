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
#include "ScenePlay.h"
#include "DeathDropBox.h"

#include "UIText.h"

const D3DXQUATERNION Character::OFFSET_ROTATION = 
    D3DXQUATERNION(0.0f, 1.0f, 0.0f, 0.0f);

const float Character::RADIUS = 30.0f;
const float Character::MAX_HEALTH = 100.0f;

Character::Character(const int index)
    : IObject(TAG_OBJECT::Character)

    // id
    , m_index(index)
    , m_cellIndex(0)
    , m_health(MAX_HEALTH)
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
    , m_killNum(0)
    , m_isKill(false)

    , m_gameOverUI()

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
    , m_isDamaged(false)
    , m_isEatEquip(false)
    , m_isGameOver(false)

    , pAnimation(nullptr)

    , m_otherHitPart(0)
    , m_stepDistance(0.0f)
{
    pAnimation = new CharacterAnimation(m_index);
    AddChild(pAnimation);
    setupAnimation();
    
    setFramePtr();

    // set boundingShapes
    m_characterParts.resize(static_cast<std::size_t>(TAG_COLLIDER_CHARACTER_PART::COUNT));
    AddPart(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::Head, this));

    for (auto pPart : m_characterParts)
        m_boundingBoxes.emplace_back(pPart->GetBoundingBoxes().front());

    const std::vector<BoundingSphere>& bss = pAnimation->GetBoundingSpheres();
    m_boundingSphere = bss.front();
    m_boundingSphere.position = GetTransform()->GetPosition();
    // end boundingShapes

    subscribeCollisionEvent();

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

    if (IsMine())
    {
        m_totalInventory.Destroy();
    }
}

void Character::OnUpdate()
{
    TAG_SCENE tagScene = CurrentScene()()->GetTagScene();

    if (tagScene == TAG_SCENE::Play)
    {
        const float receivedHealth = Communication()()->m_roomInfo.playerInfos[m_index].health;

        if (receivedHealth < m_health)
        {
            m_health = receivedHealth;
            m_isDamaged = true;
            Sound()()->Play(TAG_SOUND::Female_Hurt, Vector3::ZERO, 1.0f, FMOD_2D);
        }

        m_isDead = Communication()()->m_roomInfo.playerInfos[m_index].isDead;

        updateMine();
        updateOther();

        //Debug << "------current cell space ------ : " << m_cellIndex << endl;
    }

    // update
    GetTransform()->Update();      // set characters world
    pAnimation->UpdateAnimation(); // set characters local
    updateBone();                  // modified characters local
    pAnimation->UpdateModel();     // set characters model

    if (tagScene == TAG_SCENE::Play)
    {
        // 캐릭터와 장비 애니메이션 씽크
        syncAnimation();

        // set item animation, item model here
        updateTotalInventory();
    }

    // bounding sphere move to character position
    m_boundingSphere.position = GetTransform()->GetPosition();

    if (tagScene == TAG_SCENE::Play)
    {
        //m_bSphereSlidingCollision.position = GetTransform()->GetPosition();
        m_bBox.position = GetTransform()->GetPosition();
        m_bBox.rotation = GetTransform()->GetRotation();

        for (auto pPart : m_characterParts)
            pPart->Update();

        if (IsFire())
            RifleShooting();
    }

    //distance culling
    D3DXVECTOR3 vLength = GetTransform()->GetPosition() - CurrentCamera()()->GetPosition();
    if (D3DXVec3Length(&vLength) < Shader::SHADOW_CULLING_DISTANCE)
    {
        Shader()()->AddShadowSource(
            GetTransform()->GetTransformationMatrix(),
            pAnimation->GetSkinnedMesh());
    }

    if (tagScene == TAG_SCENE::Play)
    {
        // communication
        communicate();
    }
}

void Character::OnRender()
{
    // render
    if (CurrentCamera()()->IsObjectInsideFrustum(
        m_boundingSphere.center + m_boundingSphere.position, 
        m_boundingSphere.radius))
    {
        //distance culling
        D3DXVECTOR3 vlength = (m_boundingSphere.center + m_boundingSphere.position) - CurrentCamera()()->GetPosition();
        if (D3DXVec3Length(&vlength) < 10000.0f)
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
            static_cast<UINT>(vertices.size()),
            static_cast<UINT>(indices.size() / 2),
            indices.data(),
            D3DFMT_INDEX16,
            vertices.data(),
            sizeof vertices.front());
    });
    // end render hited box
}

void Character::updateMine()
{
    if (!IsMine()) return;

    //testing for blood particle << delete when it is done
    if (Input()()->IsOnceKeyUp(VK_END))
        ParticlePool()()->Hit_Blood(GetTransform()->GetPosition() + (Vector3::UP*170.0f), GetTransform()->GetRotation());


    if (m_isGameOver)
    {
        return;
    }
    else // m_isGameOver == false
    {
        if (m_isDead)
        {
            m_isGameOver = true;
            m_gameOverUI.Update();

            std::vector<std::pair<std::string, int>> consumes;
            TotalInventory& inven = m_totalInventory;
            for (auto& kv : inven.m_mapInventory)
            {
                std::vector<Item*>& items = kv.second;

                for (auto item : items)
                {
                    consumes.emplace_back(std::make_pair(item->GetName(), item->GetCount()));
                }
            }
            Communication()()->SendEventCreateDeathDropBox(m_index, consumes);
            CreateDeathDropBox();

            return;
        }
        else
        {
            ScenePlay* currentScene = static_cast<ScenePlay*>(Scene()()->GetCurrentScene());
            int rank = currentScene->GetSurvivors();
            if (rank == 1)
            {
                m_isGameOver = true;
                m_gameOverUI.Update();
                return;
            }
        }
    }

    //m_health 가 0이 되면 죽는 애니메이션, server communication;
    checkDead();

    const float dt = Time()()->GetDeltaTime();
    Transform* tm = GetTransform();

    //INPUT CONTROL // m_currentStayKey , m_currentOnceKey 으로 사용
    handleInput(&m_currentStayKey);
    handleInput(&m_currentOnceKey);
    if (!m_totalInventory.IsOpened() && !m_isGameOver)
    {
        handleMouse(dt, &m_mouseInput);
    }

    //m_currentState를 저장해 놓고 //dest pos 로 계 산
    D3DXVECTOR3 backupPos = tm->GetPosition();
    State destState;
    destState.position = tm->GetPosition();
    destState.rotation = tm->GetRotation();

    //*********************************

    movementControl(&destState);

    ////////////충돌 체크 Area/////////////////////
    //Terrain과의 충돌체크
    terrainFeaturesCollisionInteraction(&destState);
    //terrainFeaturesCollisionInteraction2(&destState);
    

    //D3DXMatrixOrthoLH()

    //Item 과의 충돌체크
    itemSphereCollisionInteraction();   //<<이곳 안에 m_currentOnceKey._F = false 하는 로직을 넣어놓았다(나중에 문제 생길 수 있을 것 같다)
    ////////////충돌 체크 Area/////////////////////

    setStance();
    setAttacking();
    setReload();

    if (!m_totalInventory.IsOpened())
    {
        setPunch();
    }
    
    setInteraction();
    animationControl();

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
                m_lowerAnimState,
                true,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_BLENDING_TIME,
                CharacterAnimation::DEFAULT_POSITION);

            m_savedInput = m_currentStayKey;
        }
    }
    setJump();
    
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
        backActionFrame(&rot);
    }

    // footstep
    if (m_stance != Stance::Prone)
    {
        if (m_direction == Direction::StandStill || m_Jump.isJumping)
        {
            m_stepDistance = 0.0f;
        }
        D3DXVECTOR3 v3Distance = D3DXVECTOR3(destState.position.x, 0.0f, destState.position.z) - D3DXVECTOR3(backupPos.x, 0.0f, backupPos.z);
        float fDistance = D3DXVec3Length(&v3Distance);
        m_stepDistance += fDistance;
        if (m_stepDistance >= 78.0f)
        {
            m_stepDistance -= 78.0f;
            Sound()()->Play(TAG_SOUND::FootStep, pos, 1.0f, FMOD_2D);
            Communication()()->SendEventSound(TAG_SOUND::FootStep, pos);
        }
    }
    // end footstep

    tm->SetPosition(pos);
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

        string fireModeStr = "";
        if (m_totalInventory.m_pHand->GetAuto() 
            && m_totalInventory.m_pHand->GetTagResStatic() == TAG_RES_STATIC::QBZ)
            fireModeStr = "연사";
        else
            fireModeStr = "단발";

        m_inGameUI.m_infoTextCoolDown = m_inGameUI.INFO_TEXT_COOL_TIME;
        m_inGameUI.pInfoText->SetText("발사 모드 변경: " + fireModeStr, m_inGameUI.pInfoTextShadow);
    }

    Sound()()->Listen(pos, getForward());

    //if (Input()()->IsOnceKeyDown(VK_UP))
    //{
    //    BulletPool()()->Fire(Communication()()->m_myInfo,pos + getForward() * 100.0f, getBackward(), 0.1f, 1.0f, TAG_RES_STATIC::QBZ);
    //}

    //ForDebug();
    //Debug << "current        position : " << pos << "\n\n"
    //    << "current upper animation : "
    //    << pAnimation->GetUpperAnimationName() << '\n'
    //    << "current lower animation : "
    //    << pAnimation->GetLowerAnimationName() << "\n\n";

    //여기
    //for InGameUI
    m_inGameUI.Update(m_totalInventory);

    Communication()()->SendPositionAndRotation(pos, rot);
    Communication()()->SendHeadAngle(m_headRotation.m_angle);
}

void Character::updateOther()
{
    if (IsMine()) return;

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

    //others cell space update
    IScene* pCurrentScene = CurrentScene()();
    std::size_t cellIndex = pCurrentScene->GetCellIndex(pos);
    if (cellIndex != m_cellIndex)
    {
        pCurrentScene->MoveCell(&m_cellIndex, cellIndex, TAG_OBJECT::Character, this);
    }

    D3DXVECTOR2 headAngle;
    D3DXVec2Lerp(
        &headAngle, 
        &D3DXVECTOR2(m_headRotation.m_angle, 0.0f), 
        &D3DXVECTOR2(pi.headAngle, 0.0f), 1.0f);
    m_headRotation.m_angle = headAngle.x;

    // animation
    const auto upperAnim = static_cast<TAG_ANIM_CHARACTER>(pi.upperAnimState);
    const auto lowerAnim = static_cast<TAG_ANIM_CHARACTER>(pi.lowerAnimState);
    if (upperAnim != m_upperAnimState)
    {
        m_upperAnimState = upperAnim;
        setAnimation(CharacterAnimation::BodyPart::UPPER, upperAnim, false);
    }

    if (lowerAnim != m_lowerAnimState)
    {
        m_lowerAnimState = lowerAnim;
        setAnimation(CharacterAnimation::BodyPart::LOWER, lowerAnim, false);
    }
}