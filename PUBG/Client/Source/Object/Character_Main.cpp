#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "CharacterPart.h"
#include "Bullet.h"
#include "DirectionalLight.h"
#include "AnimationState.h"
#include "Item.h"
#include "ResourceInfo.h"
#include "ComponentTransform.h"
#include "TerrainFeature.h"

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
    , m_attacking(Attacking::Unarmed)
    , m_stance(Stance::Stand)
    , m_moving(Moving::Run)
    , m_direction(Direction::Front)
    , m_upperAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_lowerAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_isFire(false)
    , m_hasChangingState(false)
    , m_isNeedRifleAnim(false)

    , pAnimation(nullptr)

    , m_otherHitPart(0)

{
    m_totalInventory.pCharacter = this;
    if (isMine())
    {
        m_totalInventory.Init();
    }

    const float factor(static_cast<float>(m_index + 1) * 100.0f);

    Transform* pTransform = GetTransform();
    pTransform->SetPosition(D3DXVECTOR3(factor, 200.0f, factor));
    pTransform->SetRotation(OFFSET_ROTATION);

    //putting character into TotalCellSpace
    IScene* CS = CurrentScene()();
    m_cellIndex = CS->GetCellIndex(pTransform->GetPosition());                   //캐릭터의 pos에 따라 알맞은 area에 넣어주기
    CS->InsertObjIntoTotalCellSpace(TAG_OBJECT::Character, m_cellIndex, this);   //Object 를 TotalCellSpace(Area)에 넣기
    CS->m_NearArea.Create(m_cellIndex);                                          //Near Area 계산

    pAnimation = new CharacterAnimation;
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

    // set item animation, item model here
    updateTotalInventory();

    // bounding sphere move to character position
    m_boundingSphere.position = GetTransform()->GetPosition();

    for (auto pPart : m_characterParts)
        pPart->Update();

    if (IsFire())
        RifleShooting();

    // render
    pAnimation->Render(
        /*m_framePtr.pWaist->CombinedTransformationMatrix
        **/ GetTransform()->GetTransformationMatrix(),
        [this](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });
    renderTotalInventory();

    for (auto pPart : m_characterParts)
        pPart->Render();

    m_boundingSphere.Render();

    //Shader::Draw(
    //    Resource()()->GetEffect("./Resource/", "Color.fx"), 
    //    nullptr, 
    //    pOtherHitPositionMesh, 
    //    0,
    //    [this](LPD3DXEFFECT pEffect)
    //{
    //    const D3DXVECTOR3& pos = m_otherHitPosition;
    //    D3DXMATRIX s, t, m;
    //    const float scale = 10.0f;
    //    D3DXMatrixScaling(&s, scale, scale, scale);
    //    D3DXMatrixTranslation(&t, pos.x, pos.y, pos.z);
    //    m = s * t;

    //    pEffect->SetMatrix(Shader::World, &m);
    //});

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
    
    // communication
    communicate();
}

void Character::OnRender()
{
}

void Character::updateMine()
{
    if (!isMine()) return;

    //Dead logic
    if (m_isDead) return;

    if (m_health <= 0.0f)
    {
        //기절 애니메이션
        TAG_ANIM_CHARACTER tagAnim = TAG_ANIM_CHARACTER::COUNT;
        if (m_stance == Stance::Stand)
            tagAnim = TAG_ANIM_CHARACTER::DBNO_Enter;
        else if (m_stance == Stance::Crouch)
            tagAnim = TAG_ANIM_CHARACTER::DBNO_Enter_From_Crouch;
        else if (m_stance == Stance::Prone)
            tagAnim = TAG_ANIM_CHARACTER::DBNO_Enter_From_Prone;

        assert((tagAnim != TAG_ANIM_CHARACTER::COUNT) && "Character::updateMine(), tagAnim is COUNT");

        setAnimation(
        CharacterAnimation::BodyPart::BOTH,
        tagAnim,
            true,
            CharacterAnimation::DEFAULT_BLENDING_TIME,
            CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            CharacterAnimation::DEFAULT_POSITION,
            0.3f,
            [this]() {
            setAnimation(
                CharacterAnimation::BodyPart::BOTH,
                TAG_ANIM_CHARACTER::DBNO_Idle,
                true,
                0.3f);
        });

        Communication()()->SendIsDead(m_index, true);
        m_isDead = true;
    }

    const float    dt = Time()()->GetDeltaTime();
    IScene*        pCurrentScene = CurrentScene()();
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

    //////////////////////////////////////////////////
    /////////////// Terrain과의 충돌체크///////////////
    //////////////////////////////////////////////////
    bool hasCollision = false;
    auto tfs(pCurrentScene->m_NearArea.GetTerrainFeatures());
    for (auto tf : tfs)
    {
        if (hasCollision) break;

        // 바운딩스피어가 충돌되지 않으면 다음 터레인피처와 충돌을 검사한다.
        if (!Collision::HasCollision(m_boundingSphere, tf->GetBoundingSphere())) continue;

        if (destState.boundingBoxes.empty())
        {
            for (auto& bb : GetBoundingBoxes())
            {
                BoundingBox destBB = bb;
                destBB.rotation = destState.rotation;
                destBB.position = destState.position;
                destState.boundingBoxes.emplace_back(destBB);
            }
        }

        for (auto& mine : destState.boundingBoxes)
        {
            if (hasCollision) break;

            for (auto& others : tf->GetBoundingBoxes())
            {
                if (hasCollision) break;

                hasCollision = Collision::HasCollision(mine, others);

                if (hasCollision)
                {
                    const D3DXVECTOR3 currPos = GetTransform()->GetPosition();
                    const D3DXVECTOR3 destPos = destState.position;
                    D3DXVECTOR3 to(destPos.x - currPos.x, 0.0f, destPos.z - currPos.z);
                    D3DXVECTOR3 dir;
                    D3DXVec3Normalize(&dir, &to);

                    D3DXVECTOR3 diff((others.center + others.position) - currPos);
                    D3DXVec3Normalize(&diff, &diff);

                    D3DXVECTOR3 right, forward;
                    D3DXMATRIX r;
                    D3DXMatrixRotationQuaternion(&r, &others.rotation);
                    D3DXVec3TransformNormal(&right, &Vector3::RIGHT, &r);
                    D3DXVec3Normalize(&right, &right);
                    D3DXVec3TransformNormal(&forward, &Vector3::FORWARD, &r);
                    D3DXVec3Normalize(&forward, &forward);

                    float dotX = D3DXVec3Dot(&right, &diff);
                    if (dotX < 0.0f)
                        dotX *= -1.0f;

                    float dotZ = D3DXVec3Dot(&forward, &diff);
                    if (dotZ < 0.0f)
                        dotZ *= -1.0f;

                    D3DXVECTOR3 dist(Vector3::ZERO);
                    float len(0.0f);
                    if (dotX > dotZ)
                    {
                        len = D3DXVec3Dot(&to, &forward);
                        dist = len * forward;
                    }
                    else
                    {
                        len = D3DXVec3Dot(&to, &right);
                        dist = len * right;
                    }

                    destState.position = currPos + dist;
                }
            }
        }
    }

    // collision with other characters
    if (!hasCollision)
    {
        for (auto o : pCurrentScene->m_NearArea.GetCharacters())
        {
            if (hasCollision) break;

            if (o->GetIndex() == m_index) continue;

            const D3DXVECTOR3 dist = destState.position - o->GetTransform()->GetPosition();
            const float distLen = D3DXVec3Length(&dist);
            if (distLen < RADIUS * 2.0f)
                hasCollision = true;
        }
    }
    // end collision /////////////////////////

    // 셋 커런트
    if (hasCollision)
    {
        // 만약 스프린트일 경우에는 쉬프트키 유무에 상관없이 런으로 바꾼다.
        if (destState.isHeadBump)
        {
            // TODO : impl
        }

        tm->SetPosition(destState.position);
    }
    else
    {
        // update state
        tm->SetPosition(destState.position);
        tm->SetRotation(destState.rotation);

        // 이사하기 //NearArea(cell space)를 다시 구하기!
        std::size_t destCellIndex = pCurrentScene->GetCellIndex(destState.position);
        if (destCellIndex != m_cellIndex)
        {
            pCurrentScene->m_NearArea.Create(destCellIndex);
            pCurrentScene->MoveCell(&m_cellIndex, destCellIndex, TAG_OBJECT::Character, this);
        }
    }
    //////////////////////////////////////////////////



    //////////////////////////////////////////////////
    /////////////// Item 과의 충돌체크/////////////////
    //////////////////////////////////////////////////
    //Item Spher와 character sphere 충돌 체크

    // clear dropped items
    auto& di = m_totalInventory.droppedItems;
    di.resize(0);

    auto itms(pCurrentScene->m_NearArea.GetItems());    //이 auto를 copy가 아닌 reference로 받는 방법은???
    for (auto itm : itms)
    {
        if (!Collision::HasCollision(m_boundingSphere, itm->GetBoundingSphere())) continue;
        //캐릭터와 Item의 spehre 가 충돌이 났다
        
        di.emplace_back(itm);
        
        // UI로 F key가 나오게 하기 


        // 잠시 주석, 인벤토리에서 보기 위해서
        //if (m_currentOnceKey._F)
        //{
        //    PutItemInTotalInventory(itm); //inventory에 넣기
        //    //current scene 에서 지우기
        //    pCurrentScene->ItemIntoInventory(pCurrentScene->GetCellIndex(itm->GetTransform()->GetPosition()), itm);
        //}
    }
    //////////////////////////////////////////////////


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

    // 카메라 프러스텀 업데이트 (왜냐하면 캐릭터0 업데이트, 렌더, 캐릭터1 업데이트, 렌더, ... 순서대로 실행되기 떄문에)
    CurrentCamera()()->UpdateFrustumCulling();
    
    D3DXVECTOR3 pos = tm->GetPosition();
    D3DXQUATERNION rot = tm->GetRotation();

    characterRotation(&m_mouseInput); //마우스 위아래 -> 케릭터 머리와 팔 위 아래로 움직임
    cameraCharacterRotation(dt, &rot, &m_mouseInput);//케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    applyTarget_Y_Position(&pos); //apply height and control jumping
    
    // shoot!
    m_isFire = false;
    m_totalInventory.m_bulletFireCoolDown -= dt;
    if (m_totalInventory.m_bulletFireCoolDown <= 0.f) m_totalInventory.m_bulletFireCoolDown = 0.f;
    if (m_attacking == Attacking::Rifle && m_currentOnceKey._LButton && !m_currentStayKey._LAlt 
        || (m_attacking == Attacking::Rifle && m_totalInventory.m_pHand->GetAuto()
        && TAG_RES_STATIC::QBZ == m_totalInventory.m_pHand->GetTagResStatic() && m_currentStayKey._LButton && !m_currentStayKey._LAlt))
    {
        if (m_totalInventory.m_bulletFireCoolDown <= 0.f &&  m_totalInventory.m_pHand->GetNumBullet() > 0)
        {
            if(m_hasChangingState == false) //장전 중일 때는 쏘지못하게
                m_isFire = true;
            //rifleShooting();
            //pistolShooting();?? 이란것도 나중에는 만들겠지요?
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

    //m_totalInventory.m_bulletFireCoolDown -= dt;
    //if (m_totalInventory.m_bulletFireCoolDown <= 0.f) m_totalInventory.m_bulletFireCoolDown = 0.f;
    //if (m_attacking == Attacking::Rifle && m_currentOnceKey._LButton && !m_currentStayKey._LAlt
    //    || (m_attacking == Attacking::Rifle && m_totalInventory.m_pHand->GetAuto()
    //        && TAG_RES_STATIC::QBZ == m_totalInventory.m_pHand->GetTagResStatic() && m_currentStayKey._LButton && !m_currentStayKey._LAlt))
    //{
    //    if (m_totalInventory.m_bulletFireCoolDown <= 0.f &&  m_totalInventory.m_pHand->GetNumBullet() > 0)
    //    {
    //        RifleShooting();
    //        backAction(&rot);
    //        //pistolShooting();?? 이란것도 나중에는 만들겠지요?
    //    }
    //}

    //if (m_backAction.Ing)
    //{
    //    backActionFrame();
    //}

    Sound()()->Listen(pos, getForward());

    if (Input()()->IsOnceKeyDown(VK_UP))
    {
        BulletPool()()->Fire(pos + getForward() * 100.0f, getBackward(), 0.1f, 1.0f, TAG_COLLISION::Player_1_Damage);

    }

    ForDebug();
    Debug << "current        position : " << pos << "\n\n"
        << "current upper animation : "
        << pAnimation->GetUpperAnimationName() << '\n'
        << "current lower animation : "
        << pAnimation->GetLowerAnimationName() << "\n\n";

    Communication()()->SendPositionAndRotation(pos, rot);
    Communication()()->SendHeadAngle(m_headRotation.m_angle);
}

void Character::updateOther()
{
    if (isMine()) return;

    auto pInput = Input()();
    auto pCom   = Communication()();
    auto pTr    = GetTransform();

    auto& pi = pCom->m_roomInfo.playerInfos[m_index];

    D3DXVECTOR3 pos;
    D3DXVec3Lerp(&pos, &pTr->GetPosition(), &pi.position, 1.0f);
    pTr->SetPosition(pos);

    D3DXQUATERNION rot;
    D3DXQuaternionSlerp(&rot, &pTr->GetRotation(), &pi.rotation, 1.0f);
    pTr->SetRotation(rot);

    D3DXVECTOR2 headAngle;
    D3DXVec2Lerp(&headAngle, &D3DXVECTOR2(m_headRotation.m_angle, 0.0f), &D3DXVECTOR2(pi.headAngle, 0.0f), 1.0f);
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