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
{
}

void Character::setFramePtr()
{
    m_framePtr.pWaist = pSkinnedMeshController->FindFrame("spine_01");
    m_framePtr.pRoot = pSkinnedMeshController->FindFrame("root");
    m_framePtr.pHandGun = pSkinnedMeshController->FindFrame("ik_hand_gun");
    m_framePtr.pTPP = pSkinnedMeshController->FindFrame("camera_tpp");
    m_framePtr.pFPP = pSkinnedMeshController->FindFrame("camera_fpp");
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

void Character::AnimationControl(OUT D3DXVECTOR3* pOut, OUT TAG_ANIM_CHARACTER* tagOut)
{
    TAG_ANIM_CHARACTER ret;

    Attacking attacking;
    Direction direction ;
    Moving    moving ;
    Stance    stance;


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
    if (m_currentInput._LShift)
    {
        moving = Moving::Sprint;
    }
    else if (m_currentInput._LCtrl)
    {
        moving = Moving::Walk;
    }
    else
    {
        moving = Moving::Run;
    }


    //Direction 8개 -----------------------------------------------------
    if (m_currentInput._W&&m_currentInput._D)
    {
        direction = Direction::FrontRight;
    }
    else if (m_currentInput._D&&m_currentInput._S)
    {
        direction = Direction::BackRight;
    }
    else if (m_currentInput._S&&m_currentInput._A)
    {
        direction = Direction::BackLeft;
    }
    else if (m_currentInput._A&&m_currentInput._W)
    {
        direction = Direction::FrontLeft;
    }
    else if (m_currentInput._W)
    {
        direction = Direction::Front;
        *pOut += getForward();

    }
    else if (m_currentInput._D)
    {
        direction = Direction::Right;
        *pOut += getRight() * m_rootTransform.MOVE_SPEED;
    }
    else if (m_currentInput._S)
    {
        direction = Direction::Back;
        *pOut += getForward() * -1.0f;
    }
    else if (m_currentInput._A)
    {
        direction = Direction::Left;
        *pOut += getRight() * -m_rootTransform.MOVE_SPEED;
    }
    else
    {
        direction = Direction::StandStill;
    }






    *tagOut = AnimationState::Get(attacking, stance, moving, direction);


    switch (m_animState)
    {
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1:
    {
        if (m_currentInput._Space)
        {
            if (m_currentInput._W)
            {
                setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_F, true);
            }
            else
            {
                setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_Stationary, true);
            }
        }
        else
        {
            if (m_currentInput._W)
            {
                if (m_currentInput._LCtrl)
                {
                    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_F, true);
                    *pOut += getForward() * 0.5f;
                }
                else if (m_currentInput._LShift)
                {
                    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F, true);
                    *pOut += getForward() * 2.0f;
                }
                else
                {
                    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_F, true);
                    *pOut += getForward();
                }
            }
            else if (m_currentInput._S)
            {
                if (m_currentInput._LCtrl)
                {
                    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_B, true);
                    *pOut += getForward() * -0.5f;
                }
                else
                {
                    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B, true);
                    *pOut += getForward() * -1.0f;
                }
            }

        }
    }
    break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_F:
    {
        if (m_currentInput._W)
        {
            *pOut += getForward() * 0.5f;
        }
        else
        {
            setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
        }
    }
    break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F:
    {
        if (m_currentInput._W)
        {
            *pOut += getForward() * 2.0f;
        }
        else
        {
            setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
        }
    }
    break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_F:
    {
        if (m_currentInput._W)
        {
            *pOut += getForward();
        }
        else
        {
            setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
        }
    }
    break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_B:
    {
        if (m_currentInput._S)
        {
            *pOut += getForward() * -0.5f;
        }
        else
        {
            setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
        }
    }
    break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B:
    {
        if (m_currentInput._S)
        {
            *pOut += getForward() * -1.0f;
        }
        else
        {
            setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
        }
    }
    break;
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

D3DXVECTOR3 Character::getForward()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::FORWARD, rot);
    dir.z *= -1.0f;
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

D3DXVECTOR3 Character::getUp()
{
    auto rot = GetTransform()->GetRotation();
    auto dir = Vector3::Rotate(Vector3::UP, rot);
    //dir.y *= -1.0f;
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
    GetTransform()->Update();
    pSkinnedMeshController->UpdateAnimation();
    updateBone();
    pSkinnedMeshController->UpdateModel();
    if (m_pRootCharacterPart) m_pRootCharacterPart->Update();

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
