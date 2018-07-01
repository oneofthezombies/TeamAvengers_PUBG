#include "stdafx.h"
#include "Character.h"
#include "SkinnedMeshController.h"
#include "CharacterPart.h"
#include "DirectionalLight.h"
#include "AnimationState.h"

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
    , _Z(false)
    , _X(false)
    , _C(false)
    , _Space(false)
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

void Character::setFramePtr()
{
    m_framePtr.pWaist = pSkinnedMeshController->FindFrame("spine_01");
    m_framePtr.pRoot = pSkinnedMeshController->FindFrame("root");
    m_framePtr.pHandGun = pSkinnedMeshController->FindFrame("ik_hand_gun");
    m_framePtr.pTPP = pSkinnedMeshController->FindFrame("camera_tpp");
    m_framePtr.pFPP = pSkinnedMeshController->FindFrame("camera_fpp");
    m_framePtr.pSlotPrimary = pSkinnedMeshController->FindFrame("slot_primary");
    m_framePtr.pSlotSecondary = pSkinnedMeshController->FindFrame("slot_secondary");
    m_framePtr.pSlotMelee = pSkinnedMeshController->FindFrame("slot_melee");
    m_framePtr.pSlotThrowable = pSkinnedMeshController->FindFrame("slot_throwable");
}

void Character::subscribeCollisionEvent()
{
    if (isMine())
    {
        auto tagBody = GetTagCollisionBody(m_index);
        for (int i = 0; i < NUM_PLAYER; ++i)
        {
            if (i == m_index) continue;

            Collision()()->SubscribeCollisionEvent(
                GetTagCollisionDamage(i), tagBody);
        }
    }
}

Character::IsPressing Character::HandleInput(IsPressing& m_isPressing)
{
    InputManager* pInput = Input()();

    m_isPressing._LAlt = pInput->IsStayKeyDown(VK_LMENU);
    m_isPressing._LCtrl = pInput->IsStayKeyDown(VK_LCONTROL);
    m_isPressing._LShift = pInput->IsStayKeyDown(VK_LSHIFT);
    m_isPressing._W = pInput->IsStayKeyDown('W');
    m_isPressing._S = pInput->IsStayKeyDown('S');
    m_isPressing._A = pInput->IsStayKeyDown('A');
    m_isPressing._D = pInput->IsStayKeyDown('D');
    m_isPressing._Z = pInput->IsStayKeyDown('Z');
    m_isPressing._X = pInput->IsStayKeyDown('X');
    m_isPressing._C = pInput->IsStayKeyDown('C');
    m_isPressing._Space = pInput->IsOnceKeyDown(VK_SPACE);

    return m_isPressing;
}
void Character::CameraCharacterRotation(OUT D3DXQUATERNION* rOut)
{
    ICamera*       pCurrentCamera = CurrentCamera()();
    const float dt = Time()()->GetDeltaTime();

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
    if (m_currentInput._LAlt)
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
        // update rotation of transform

        D3DXQUATERNION q;
        D3DXQuaternionRotationYawPitchRoll(&q, yaw, 0.0f, 0.0f);
        //cout << "pitch : " << pitch << " yaw : " << yaw << endl;

        *rOut *= q;
        m_rotationForCamera.x += -pitch;
        // reset rotFotCameraTP
        if (tempBool)
        {
            m_rotationForCamera = Vector3::ZERO;
            tempBool = false;
        }

    }

    /*
    in : mouse
    out : character r <= mouse only one axis
    out : camera r <= mouse wto axis
    */

    POINT center;
    center.x = 1280 / 2;
    center.y = 720 / 2;
    ClientToScreen(g_hWnd, &center);
    SetCursorPos(center.x, center.y);
}

void Character::AnimationMovementControl(OUT D3DXVECTOR3* pOut, OUT TAG_ANIM_CHARACTER* tagOut)
{
    TAG_ANIM_CHARACTER ret;

    Attacking attacking;
    Direction direction ;
    Moving    moving ;
    Stance    stance;

    float movingFactor;


    //Attacking 3개 -----------------------------------------------------
    if (false/*이곳에는 아이템이 껴 있는지 없는지를 확인해서 넣기*/)
    {
        attacking = Attacking::Rifle;
    }
    else if (false/*이곳에는 아이템이 껴 있는지 없는지를 확인해서 넣기*/)
    {
        attacking = Attacking::Melee;
    }
    else
    {
        attacking = Attacking::Unarmed;
    }

    //Stance 3개 -----------------------------------------------------
    if (m_currentInput._C)
    {
        stance = Stance::Crouch;
    }
    else if (m_currentInput._Z)
    {
        stance = Stance::Prone;
    }
    else
    {
        stance = Stance::Stand;
    }



    //Moving 3개 -----------------------------------------------------
    if (m_currentInput._LShift && !m_currentInput._LCtrl)
    {
        moving = Moving::Sprint;
        movingFactor = 2.0f;
    }
    else if (m_currentInput._LCtrl && !m_currentInput._LShift)
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
    if (m_currentInput._W&&m_currentInput._D)
    {
        direction = Direction::FrontRight;
        *pOut += getForwardRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentInput._D&&m_currentInput._S)
    {
        direction = Direction::BackRight;
        *pOut += getBackwardRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentInput._S&&m_currentInput._A)
    {
        direction = Direction::BackLeft;
        *pOut += getBackwardLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentInput._A&&m_currentInput._W)
    {
        direction = Direction::FrontLeft;
        *pOut += getForwardLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentInput._W)
    {
        direction = Direction::Front;
        *pOut += getForward() * movingFactor * m_rootTransform.MOVE_SPEED;

    }
    else if (m_currentInput._D)
    {
        direction = Direction::Right;
        *pOut += getRight() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentInput._S)
    {
        direction = Direction::Back;
        //*pOut += getForward() * -1.0f;
        *pOut += getBackward() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentInput._A)
    {
        direction = Direction::Left;
        *pOut += getLeft() * movingFactor * m_rootTransform.MOVE_SPEED;
    }
    else
    {
        direction = Direction::StandStill;
    }




    if (tagOut) //if null, no changes in animation
    {
        *tagOut = AnimationState::Get(attacking, stance, moving, direction);
    }
        
}
bool Character::isMine() const
{
    return m_index == Communication()()->m_MyInfo.m_ID;
}

void Character::setAnimation(
    const TAG_ANIM_CHARACTER tag, const bool isBlend, 
    const float currentWeight, const float nextWeight, const float blendTime)
{
    assert(pSkinnedMeshController && 
        "Character::setAnimation(), skinned mesh controller is null.");

    pSkinnedMeshController->SetAnimation(
        TagAnimToString::Get(tag), 
        isBlend, 
        currentWeight, 
        nextWeight, 
        blendTime);

    m_animState = tag;
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

    //TODO 애니메이션 다시 돌아오게 하기
    // for root motion animation
    m_framePtr.pRoot->TransformationMatrix = Matrix::IDENTITY;
}

void Character::updateDependency()
{
    // update
    GetTransform()->Update();
    pSkinnedMeshController->UpdateAnimation();
    updateBone();
    pSkinnedMeshController->UpdateModel();
    updateTotalInventory();
    if (m_pRootCharacterPart) m_pRootCharacterPart->Update();

    // render
    pSkinnedMeshController->Render(
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
    if (m_pRootCharacterPart) m_pRootCharacterPart->Render();
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
