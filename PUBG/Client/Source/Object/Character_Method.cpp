#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "CharacterPart.h"
#include "DirectionalLight.h"
#include "AnimationState.h"
#include "Item.h"
#include "ItemInfo.h"
#include "ComponentTransform.h"
#include "HeightMap.h"
#include "ScenePlay.h"
#include "Ballistics.h"

Character::WaistRotation::WaistRotation(const float limit, const float factor)
    : LIMIT_OF_ANGLE(limit)
    , QUANTITY_FACTOR(factor)
    , m_angle(0.0f)
{
}

Character::RootTransform::RootTransform(const float moveSpeed)
    : MOVE_SPEED(moveSpeed)
{
}
Character::Info::Info()
    : pTransform(nullptr)
    , pRotationForCamera(nullptr)
    , pFPP(nullptr)
    , pTPP(nullptr)
{
}

Character::IsPressing::IsPressing()
    : _LAlt(false)
    , _LCtrl(false)
    , _LShift(false)
    , _W(false)
    , _S(false)
    , _A(false)
    , _D(false)
{
}

bool Character::IsPressing::operator==(const IsPressing& other) const
{
    if (_LAlt != other._LAlt) return false;
    if (_LCtrl != other._LCtrl) return false;
    if (_LShift != other._LShift) return false;
    if (_W != other._W) return false;
    if (_S != other._S) return false;
    if (_A != other._A) return false;
    if (_D != other._D) return false;

    return true;
}

bool Character::IsPressing::operator!=(const IsPressing& other) const
{
    return !(*this == other);
}

Character::IsPressed::IsPressed()
    : _Z(false)
    , _X(false)
    , _C(false)
    , _R(false)
    , _F(false)
    , _Space(false)
    , _Num1(false)
    , _Num2(false)
    , _Num3(false)
    , _Num4(false)
    , _Num5(false)
    , _LButton(false)
    , _RButton(false)
{

}

Character::FramePtr::FramePtr()
    : pRoot(nullptr)
    , pWaist(nullptr)
    , pHandGun(nullptr)
    , pTPP(nullptr)
    , pFPP(nullptr)
    , pSlotPrimary(nullptr)
    , pSlotSecondary(nullptr)
    , pSlotMelee(nullptr)
    , pSlotThrowable(nullptr)
{
}

Character::IsJumping::IsJumping()
    : isJumping(false)
    
    , jumpPower(12.0f)
    , gravity(0.3f)
    , currGravity(0.0f)
    , maxStepHeight(5.f)
{
}

Character::State::State()
    : position(Vector3::ZERO)
    , rotation(Vector3::ZERO)
    , isHeadBump(false)
{
    D3DXMatrixIdentity(&transformationMatrix);
}

void Character::setFramePtr()
{
    m_framePtr.pWaist         = pAnimation->FindFrame("spine_01");
    m_framePtr.pRoot          = pAnimation->FindFrame("root");
    m_framePtr.pHandGun       = pAnimation->FindFrame("ik_hand_gun");
    m_framePtr.pTPP           = pAnimation->FindFrame("camera_tpp");
    m_framePtr.pFPP           = pAnimation->FindFrame("camera_fpp");
    m_framePtr.pSlotPrimary   = pAnimation->FindFrame("slot_primary");
    m_framePtr.pSlotSecondary = pAnimation->FindFrame("slot_secondary");
    m_framePtr.pSlotMelee     = pAnimation->FindFrame("slot_melee");
    m_framePtr.pSlotThrowable = pAnimation->FindFrame("slot_throwable");

    FramePtr& p = m_framePtr;
    assert(
        p.pWaist &&
        p.pRoot &&
        p.pHandGun &&
        p.pTPP &&
        p.pFPP &&
        p.pSlotPrimary &&
        p.pSlotSecondary &&
        p.pSlotMelee &&
        p.pSlotThrowable && "Character::setFramePtr(), pointer is null.");
}

void Character::subscribeCollisionEvent()
{
    if (isMine())
    {
        auto tagBody = GetTagCollisionBody(m_index);
        for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
        {
            if (i == m_index) continue;

            Collision()()->AddCollisionEvent(
                GetTagCollisionDamage(i), 
                tagBody);
        }
    }
}

void Character::handleInput(IsPressing* OutIsPressing)
{
    assert(OutIsPressing && "Character::handleInput(IsPressing), argument is null.");

    InputManager* pInput = Input()();

    OutIsPressing->_LAlt   = pInput->IsStayKeyDown(VK_LMENU);
    OutIsPressing->_LCtrl  = pInput->IsStayKeyDown(VK_LCONTROL);
    OutIsPressing->_LShift = pInput->IsStayKeyDown(VK_LSHIFT);
    OutIsPressing->_W      = pInput->IsStayKeyDown('W');
    OutIsPressing->_S      = pInput->IsStayKeyDown('S');
    OutIsPressing->_A      = pInput->IsStayKeyDown('A');
    OutIsPressing->_D      = pInput->IsStayKeyDown('D');
}

void Character::handleInput(IsPressed* OutIsPressed)
{
    assert(OutIsPressed && "Character::handleInput(IsPressed), argument is null.");

    InputManager* pInput = Input()();

    OutIsPressed->_Z            = pInput->IsOnceKeyDown('Z');
    OutIsPressed->_X            = pInput->IsOnceKeyDown('X');
    OutIsPressed->_C            = pInput->IsOnceKeyDown('C');
    OutIsPressed->_R            = pInput->IsOnceKeyDown('R');
    OutIsPressed->_F            = pInput->IsOnceKeyDown('F');
    OutIsPressed->_Space        = pInput->IsOnceKeyDown(VK_SPACE);
    OutIsPressed->_Num1         = pInput->IsOnceKeyDown('1');
    OutIsPressed->_Num2         = pInput->IsOnceKeyDown('2');
    OutIsPressed->_Num3         = pInput->IsOnceKeyDown('3');
    OutIsPressed->_Num4         = pInput->IsOnceKeyDown('4');
    OutIsPressed->_Num5         = pInput->IsOnceKeyDown('5');
    OutIsPressed->_LButton      = pInput->IsOnceKeyDown(VK_LBUTTON);
    OutIsPressed->_RButton      = pInput->IsOnceKeyDown(VK_RBUTTON);
}

void Character::cameraCharacterRotation(const float dt, D3DXQUATERNION* OutRotation)
{
    ICamera* pCurrentCamera = CurrentCamera()();

    // get mouse pos
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(g_hWnd, &mouse);

    POINT diff;
    diff.x = mouse.x - 1280 / 2;
    diff.y = mouse.y - 720 / 2;
    if (diff.x < 2 && diff.x > -2) diff.x = 0;
    if (diff.y < 2 && diff.y > -2) diff.y = 0;
    const float yaw = diff.x * 0.2f * dt;
    const float pitch = diff.y * 0.2f * dt;

    static bool tempBool = false;
    if (m_currentStayKey._LAlt)
    {
        if (pCurrentCamera)
        {
            if (pCurrentCamera->GetTagCamera() == TAG_CAMERA::Third_Person || pCurrentCamera->GetTagCamera() == TAG_CAMERA::Default)
            {
                m_rotationForCamera.x += -pitch;
                m_rotationForCamera.y += yaw;
                tempBool = true;
            }
        }
    }
    else // isPressing_LAlt == false
    {
        D3DXQUATERNION q;
        D3DXQuaternionRotationYawPitchRoll(&q, yaw, 0.0f, 0.0f);
        *OutRotation *= q;
        m_rotationForCamera.x += -pitch;
        // reset rotFotCameraTP
        if (tempBool)
        {
            m_rotationForCamera = Vector3::ZERO;
            tempBool = false;
        }
    }

    static bool test_sound = true;

    if (Input()()->IsOnceKeyDown(VK_LEFT))
        test_sound = true;
    if (Input()()->IsOnceKeyDown(VK_RIGHT))
    {
        test_sound = false;
    }
    if (test_sound)
    {
        POINT center;
        center.x = 1280 / 2;
        center.y = 720 / 2;
        ClientToScreen(g_hWnd, &center);
        SetCursorPos(center.x, center.y);
    }

}

void Character::animationMovementControl(OUT State* OutState, TAG_ANIM_CHARACTER* OutTag)
{
    Direction direction;
    Moving    moving;

    float movingFactor;

    //점프
    if (m_currentOnceKey._Space)
        m_Jump.isJumping = true;

    //Moving 3개 -----------------------------------------------------
    /*
    Prone의 경우 Sprint가 없고
    Stand, Crouch의 경우 BR, B, BL일 때 Sprint가 없다
    */
    if (m_currentStayKey._LShift && !m_currentStayKey._LCtrl) 
    {
        moving = Moving::Sprint;
        movingFactor = 2.0f;
    }
    else if (m_currentStayKey._LCtrl && !m_currentStayKey._LShift)
    {
        moving = Moving::Walk;
        movingFactor = 0.5f;
    }
    else
    {
        moving = Moving::Run;
        movingFactor = 1.0f;
    }

    //Direction 8개 -----------------------------------------------------
    if (m_currentStayKey._W&&m_currentStayKey._D)
    {
        direction = Direction::FrontRight;
        OutState->position += getForwardRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._D&&m_currentStayKey._S)
    {
        direction = Direction::BackRight;
        OutState->position += getBackwardRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._S&&m_currentStayKey._A)
    {
        direction = Direction::BackLeft;
        OutState->position += getBackwardLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._A&&m_currentStayKey._W)
    {
        direction = Direction::FrontLeft;
        OutState->position += getForwardLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._W)
    {
        direction = Direction::Front;
        OutState->position += getForward() * movingFactor * m_rootTransform.MOVE_SPEED;

    }
    else if (m_currentStayKey._D)
    {
        direction = Direction::Right;
        OutState->position += getRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._S)
    {
        direction = Direction::Back;
        //*pOut += getForward() * -1.0f;
        OutState->position += getBackward() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._A)
    {
        direction = Direction::Left;
        OutState->position += getLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else
    {
        direction = Direction::StandStill;
    }

    if (OutTag) //if null, no changes in animation
    {
        *OutTag = AnimationState::Get(m_attacking, m_stance, moving, direction);
    }
}

bool Character::isMine() const
{
    return m_index == Communication()()->m_MyInfo.m_ID;
}

void Character::applyTarget_Y_Position(OUT D3DXVECTOR3 * pOut)
{  
    HeightMap* pCurrentScene = CurrentScene()()->GetHeightMap();
    if (!pCurrentScene)
        return; //assert(false && "No CurrentScene");

    D3DXVECTOR3 targetPos = *pOut;
    float height = 0;
    bool isIntersected = true;

    if (m_Jump.isJumping)
    {
        targetPos.y += m_Jump.jumpPower - m_Jump.currGravity;
        m_Jump.currGravity += m_Jump.gravity;
        
        isIntersected = pCurrentScene->GetHeight(targetPos, &height);

        //if (!isIntersected)
        //{
        //    // Do nothing just yet

        //}
        //else
        //{

        //}

        if (targetPos.y <= height && m_Jump.jumpPower < m_Jump.currGravity)
        {
            targetPos.y = height;
            m_Jump.isJumping = false;
            m_Jump.currGravity = 0.0f;
        }
    }
    else //when no jump
    {
        isIntersected = pCurrentScene->GetHeight(targetPos, &height);
        if (!isIntersected /*|| fabs(height - *pOut.y) > m_Jump.maxStepHeight*/)
        {
            //Do nothing
        }
        else
        {
            targetPos.y = height;
        }
    }
    *pOut = targetPos;
}

void Character::rifleShooting()
{
    auto& inven = m_totalInventory;
    inven.m_bulletFireCoolDown = ItemInfo::GetBulletFireCoolTime(inven.m_pHand->GetTagResStatic());

    int numBullet = inven.m_pHand->GetNumBullet();
    inven.m_pHand->SetNumBullet(--numBullet);
    cout << "총에 남아있는 총알 개수: " << inven.m_pHand->GetNumBullet() << "\n";

    //Goal : get Fire starting position , get fire direction
    inven.m_pHand->UpdateModel(); //update to get position of frame "gun_bolt" 
    D3DXMATRIX mat =
        inven.m_pHand->GetGunBolt()->CombinedTransformationMatrix  //model space combinde matrix
        * m_framePtr.pHandGun->CombinedTransformationMatrix // hand gun space matrix
        * GetTransform()->GetTransformationMatrix();    //character world matrix
    
    D3DXVECTOR3 bulletFirePos = Matrix::GetTranslation(mat);
    
    D3DXVECTOR3 bulletDir;
    CurrentCamera()()->CalcPickedPosition(&bulletDir, 1280 / 2, 720 / 2);
    bulletDir = bulletDir - bulletFirePos;
    D3DXVec3Normalize(&bulletDir, &bulletDir);
    //-------------------------

    //// ----------------------수정중-------------
    //// 나 말고 다른 캐릭터들을 순회하며 충돌여부를 검사한다.
    //const auto& others = static_cast<ScenePlay*>(CurrentScene()())->GetOthers();
    //for (auto o : others)
    //{
    //    Transform* oTr = o->GetTransform();
    //    const D3DXVECTOR3 oPos = oTr->GetPosition();

    //    // 먼저 캐릭터의 바운딩스피어와 충돌을 검사한다.
    //    BoundingSphere bs = o->GetBoundingSphere()->CopyTo(oPos);
    //    if (!D3DXSphereBoundProbe(&bs.center, bs.radius, &bulletFirePos, &bulletDir)) continue;

    //    const float distance = D3DXVec3Length(&(oPos - bulletFirePos));

    //    // 탄도학에 의한 예측 낙차 높이를 계산한다.
    //    const float varY = Ballistics::GetVarianceY(inven.m_pHand->GetTagResStatic(), distance);
    //    D3DXVECTOR3 estimatedDest = bulletDir * distance;
    //    estimatedDest.y += varY;
    //    
    //    // 낙차 높이를 계산한 곳으로 레이를 쏜다.
    //    bulletDir = estimatedDest - bulletFirePos;
    //    D3DXVec3Normalize(&bulletDir, &bulletDir);

    //    // 캐릭터의 바운딩박스들과 충돌을 검사한다.
    //    std::vector<BoundingBox> bbs;
    //    //BoundingBox hitBox;
    //    float hitBoxDist = std::numeric_limits<float>::max();
    //    for (auto obb : o->GetBoundingBoxes())
    //    {
    //        BoundingBox bb = obb->CopyTo(oTr->GetTransformationMatrix());
    //        
    //        D3DXVECTOR3 min(-Vector3::ONE);
    //        D3DXVECTOR3 max(Vector3::ONE);
    //        std::vector<D3DXVECTOR3> vertices(8);
    //        vertices[0] = D3DXVECTOR3(min.x, min.y, min.z);
    //        vertices[1] = D3DXVECTOR3(min.x, max.y, min.z);
    //        vertices[2] = D3DXVECTOR3(max.x, max.y, min.z);
    //        vertices[3] = D3DXVECTOR3(max.x, min.y, min.z);
    //        vertices[4] = D3DXVECTOR3(min.x, min.y, max.z);
    //        vertices[5] = D3DXVECTOR3(min.x, max.y, max.z);
    //        vertices[6] = D3DXVECTOR3(max.x, max.y, max.z);
    //        vertices[7] = D3DXVECTOR3(max.x, min.y, max.z);

    //        D3DXMATRIX s, m;
    //        D3DXMatrixScaling(&s, bb.extent.x, bb.extent.y, bb.extent.z);
    //        m = s * bb.transformationMatrix;

    //        for (auto& v : vertices)
    //            D3DXVec3TransformCoord(&v, &v, &m);

    //        std::vector<D3DXPLANE> planes(6);
    //        //근평면//좌상전//우상전//좌하전
    //        D3DXPlaneFromPoints(&planes[0], &vertices[0], &vertices[1], &vertices[2]);
    //        //원평면//우상후//좌상후//우하후
    //        D3DXPlaneFromPoints(&planes[1], &vertices[7], &vertices[6], &vertices[5]);
    //        //좌평면//좌상후//좌상전//좌하후
    //        D3DXPlaneFromPoints(&planes[2], &vertices[4], &vertices[5], &vertices[1]);
    //        //우평면//우상전//우상후//우하전
    //        D3DXPlaneFromPoints(&planes[3], &vertices[3], &vertices[2], &vertices[6]);
    //        //상평면//좌상후//우상후//좌상전
    //        D3DXPlaneFromPoints(&planes[4], &vertices[1], &vertices[5], &vertices[6]);
    //        //하평면//좌하전//우하전//좌하후
    //        D3DXPlaneFromPoints(&planes[5], &vertices[4], &vertices[0], &vertices[3]);

    //        D3DXVECTOR3 point;
    //        for (auto p : planes)
    //        {
    //            if (D3DXPlaneIntersectLine(&point, &p, &bulletFirePos, &estimatedDest))
    //            {
    //                // hit
    //                const float bbDist = D3DXVec3Length(&(bb.center - bulletFirePos));
    //                if (bbDist < hitBoxDist)
    //                {
    //                    hitBoxDist = bbDist;
    //                    //hitBox = bb;
    //                    m_otherHitedBox = bb;
    //                }
    //                break;
    //            }
    //        }
    //    }

    //    if (hitBoxDist != std::numeric_limits<float>::max())
    //    {
    //        // 제일 작은 히트한 놈
    //        
    //    }
    //}
    //// ----------------------수정중-------------

    switch (inven.m_pHand->GetTagResStatic())
    {
    case TAG_RES_STATIC::QBZ:
        BulletPool()()->Fire(bulletFirePos, bulletDir, ItemInfo::GetInitialBulletSpeed(TAG_RES_STATIC::QBZ), ItemInfo::GetBaseDamage(TAG_RES_STATIC::QBZ), TAG_COLLISION::Impassable);
        break;
    case TAG_RES_STATIC::Kar98k:
        BulletPool()()->Fire(bulletFirePos, bulletDir, ItemInfo::GetInitialBulletSpeed(TAG_RES_STATIC::Kar98k), ItemInfo::GetBaseDamage(TAG_RES_STATIC::Kar98k), TAG_COLLISION::Impassable);
        break;
    }
}

void Character::movementControl(OUT State* OutState)
{
    float movingFactor;

    //점프
    if (m_currentOnceKey._Space)
        m_Jump.isJumping = true;

    //Moving 3개 -----------------------------------------------------
    /*
    Prone의 경우 Sprint가 없고
    Stand, Crouch의 경우 BR, B, BL일 때 Sprint가 없다
    */
    if (m_currentStayKey._LShift && !m_currentStayKey._LCtrl)
    {
        movingFactor = 2.0f;
    }
    else if (m_currentStayKey._LCtrl && !m_currentStayKey._LShift)
    {
        movingFactor = 0.5f;
    }
    else
    {
        movingFactor = 1.0f;
    }

    //Direction 8개 -----------------------------------------------------
    if (m_currentStayKey._W&&m_currentStayKey._D)
    {
        OutState->position += getForwardRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._D&&m_currentStayKey._S)
    {
        OutState->position += getBackwardRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._S&&m_currentStayKey._A)
    {
        OutState->position += getBackwardLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._A&&m_currentStayKey._W)
    {
        OutState->position += getForwardLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._W)
    {
        OutState->position += getForward() * movingFactor * m_rootTransform.MOVE_SPEED;

    }
    else if (m_currentStayKey._D)
    {
        OutState->position += getRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._S)
    {
        OutState->position += getBackward() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentStayKey._A)
    {
        OutState->position += getLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
}

void Character::animationControl()
{
    Direction direction;
    Moving    moving;

    //Moving 3개 -----------------------------------------------------
    /*
    Prone의 경우 Sprint가 없고
    Stand, Crouch의 경우 BR, B, BL일 때 Sprint가 없다
    */
    if (m_currentStayKey._LShift && !m_currentStayKey._LCtrl)
    {
        moving = Moving::Sprint;
    }
    else if (m_currentStayKey._LCtrl && !m_currentStayKey._LShift)
    {
        moving = Moving::Walk;
    }
    else
    {
        moving = Moving::Run;
    }

    //Direction 8개 -----------------------------------------------------
    if (m_currentStayKey._W&&m_currentStayKey._D)
    {
        direction = Direction::FrontRight;
    }
    else if (m_currentStayKey._D&&m_currentStayKey._S)
    {
        direction = Direction::BackRight;
    }
    else if (m_currentStayKey._S&&m_currentStayKey._A)
    {
        direction = Direction::BackLeft;
    }
    else if (m_currentStayKey._A&&m_currentStayKey._W)
    {
        direction = Direction::FrontLeft;
    }
    else if (m_currentStayKey._W)
    {
        direction = Direction::Front;

    }
    else if (m_currentStayKey._D)
    {
        direction = Direction::Right;
    }
    else if (m_currentStayKey._S)
    {
        direction = Direction::Back;
    }
    else if (m_currentStayKey._A)
    {
        direction = Direction::Left;
    }
    else
    {
        direction = Direction::StandStill;
    }

    m_lowerAnimState = AnimationState::Get(m_attacking, m_stance, moving, direction);
}

void Character::setInfo()
{
    m_info.pTransform = GetTransform();
    m_info.pRotationForCamera = &m_rotationForCamera;
    m_info.pTPP = m_framePtr.pTPP;
    m_info.pFPP = m_framePtr.pFPP;
}

D3DXVECTOR3 Character::getUp()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::UP, rot);
    //dir.y *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getForward()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getBackward()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD, rot);
    dir.z *= -1.0f; 
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::LEFT, rot);
    dir.x *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::RIGHT, rot);
    dir.x *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getForwardLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD_RIGNT, rot);//왜 forward_Right인지..?? 이래야 잘 돌아 가는 이유는?
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getForwardRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD_LEFT, rot); //왜 forward_left인지..?? 이래야 잘 돌아 가는 이유는?
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getBackwardRight()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD_LEFT, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

D3DXVECTOR3 Character::getBackwardLeft()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::BACKWARD_RIGHT, rot);
    dir.z *= -1.0f;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

void Character::updateBone()
{
    // modify local bones
    D3DXMATRIX rx;
    D3DXMatrixRotationX(&rx, m_waistRotation.m_angle);
    m_framePtr.pWaist->TransformationMatrix *= rx;

    // for root motion animation
    m_framePtr.pRoot->TransformationMatrix = Matrix::IDENTITY;
}

void Character::updateDependency()
{
    // update
    GetTransform()->Update();
    pAnimation->UpdateAnimation();
    updateBone();
    pAnimation->UpdateModel();
    updateTotalInventory();

    for (auto pPart : m_characterParts)
        pPart->Update();

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

    m_pBoundingSphere->CopyTo(GetTransform()->GetPosition()).Render();

    Shader::Draw(Resource()()->GetEffect("./Resource/", "Color.fx"), nullptr, pOtherHitPointMesh, 0, 
        [this](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(
            Shader::World, &Matrix::IDENTITY);
    });
}

void Character::communicate()
{
    //if (isMine())
    //{
    //    if (isFired)
    //    {
    //        D3DXQUATERNION rot;
    //        D3DXQuaternionRotationAxis(&rot, &Vector3::UP, D3DX_PI * 0.5f);
    //        auto bullet = BulletPool()()->Fire(Matrix::GetTranslation(
    //            m_framePtr.pHandGun->CombinedTransformationMatrix
    //            * tr->GetTransformationMatrix()),
    //            rot, 0.1f, 10.0f, GetTagCollisionDamage(m_index));
    //        pCom->SendEventFireBullet(bullet);
    //    }

    //    if (isUpdated)
    //        pCom->SendPosition(pos);
    //}
}

void Character::rotateWaist(const float quantity)
{
    auto& wr = m_waistRotation;

    wr.m_angle += quantity;

    if (wr.m_angle < -wr.LIMIT_OF_ANGLE)
        wr.m_angle = -wr.LIMIT_OF_ANGLE;
    else if (wr.m_angle > wr.LIMIT_OF_ANGLE)
        wr.m_angle = wr.LIMIT_OF_ANGLE;
}

int Character::GetIndex() const
{
    return m_index;
}

TAG_COLLISION Character::GetTagCollisionBody(const int index)
{
    switch (index) {
    case 0: return TAG_COLLISION::Player_0_Body;
    case 1: return TAG_COLLISION::Player_1_Body;
    case 2: return TAG_COLLISION::Player_2_Body;
    case 3: return TAG_COLLISION::Player_3_Body;
    default: {
            assert(false &&
                "Character::GetTagCollisionBody() failed. index is wrong.");
            return TAG_COLLISION::Idle;
        }
    }
}

TAG_COLLISION Character::GetTagCollisionDamage(const int index)
{
    switch (index) {
    case 0: return TAG_COLLISION::Player_0_Damage;
    case 1: return TAG_COLLISION::Player_1_Damage;
    case 2: return TAG_COLLISION::Player_2_Damage;
    case 3: return TAG_COLLISION::Player_3_Damage;
    default: {
            assert(false &&
                "Character::GetTagCollisionDamage() failed. index is wrong.");
            return TAG_COLLISION::Idle;
        }
    }
}

CharacterAnimation* Character::GetCharacterAnimation()
{
    return pAnimation;
}

const std::vector<BoundingBox*>& Character::GetBoundingBoxes()
{
    return m_boundingBoxes;
}

void Character::AddPart(CharacterPart* pPart)
{
    m_characterParts[static_cast<std::size_t>(pPart->GetTagColliderCharacterPart())] = pPart;
}

D3DXVECTOR3 Character::GetWaistPosition()
{
    return Matrix::GetTranslation(m_framePtr.pWaist->CombinedTransformationMatrix * GetTransform()->GetTransformationMatrix());
}
