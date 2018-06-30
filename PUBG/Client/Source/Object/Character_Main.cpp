#include "stdafx.h"
#include "Character.h"
#include "CharacterPart.h"
#include "Bullet.h"
#include "SkinnedMeshController.h"
#include "DirectionalLight.h"

const D3DXQUATERNION Character::OFFSET_ROTATION = 
    D3DXQUATERNION(0.0f, 1.0f, 0.0f, 0.0f);

Character::Character(const int index)
    : IObject()

    , m_index(index)
    , m_animState(TAG_ANIM_CHARACTER::Rifle_Combat_Prone_PrimarySlot)
    , m_rootTransform(1.0f)
    , m_waistRotation(D3DX_PI * 0.5f, 0.1f)
    , m_framePtr()
    , m_pSphereMesh(nullptr)
    , m_pRootCharacterPart(nullptr)
    , m_totalInventory()

    , pSkinnedMeshController(nullptr)
    , pTargetTransform(nullptr)
{
    Transform* tr = GetTransform();
    tr->SetRotation(OFFSET_ROTATION);
    tr->SetPosition(D3DXVECTOR3(static_cast<float>(index) * 100.0f, 0.0f, 0.0f));

    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
    pSkinnedMeshController->SetSkinnedMesh(
        Resource()()->GetCharacterSkinnedMesh());
    
    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false);

    setFramePtr();

    m_pRootCharacterPart =
        new CharacterPart(TAG_COLLIDER_CHARACTER_PART::Head, this);

    // set sphere mesh for root position
    D3DXCreateSphere(Device()(), 1.0f, 10, 10, &m_pSphereMesh, nullptr);

    subscribeCollisionEvent();

    if (isMine())
    {
        setInfo();
        Camera()()->SetTarget(&m_info);
    }

    m_rotationForCamera = Vector3::ZERO;
}

Character::~Character()
{
    SAFE_DELETE(m_pRootCharacterPart);
    SAFE_RELEASE(m_pSphereMesh);
}

void Character::OnUpdate()
{
    updateMine();
    updateOther();
    updateDependency();
    communicate();
}

void Character::OnRender()
{
    auto pD = Device()();
    pD->SetRenderState(D3DRS_LIGHTING, true);
     
    pD->SetTexture(0, NULL);
    pD->SetMaterial(&MaterialTemplate::GetWhite());
    pD->SetTransform(
        D3DTS_WORLD, &GetTransform()->GetTransformationMatrix());
    m_pSphereMesh->DrawSubset(0);

    // for distance measurement
    for (int i = 0; i < 10; ++i)
    {
        D3DXMATRIX t;
        D3DXMatrixTranslation(&t, static_cast<float>(i * 10), 0.0f, 0.0f);
        pD->SetTransform(D3DTS_WORLD, &t);
        m_pSphereMesh->DrawSubset(0);
    }
}

void Character::updateMine()
{
    if (!isMine()) return;

    // for anim debug
    //auto animIndex = pSkiCon->GetCurrentAnimationIndex();
    //const auto numAnim = pSkiCon->GetNumAnimation();

    //if (pInput->IsOnceKeyDown('1'))
    //{
    //    if (animIndex > 0)
    //        animIndex--;

    //    pSkiCon->SetAnimationIndex(animIndex, true);
    //}
    //if (pInput->IsOnceKeyDown('2'))
    //{
    //    if (animIndex < numAnim - 1)
    //        animIndex++;

    //    pSkiCon->SetAnimationIndex(animIndex, true);
    //}

    //Debug << "num Anim : " << numAnim << ", anim index : " << animIndex << '\n';

    //if (pInput->IsStayKeyDown('3'))
    //{
    //    RotateWaist(-m_WaistRotation.QUANTITY_FACTOR);
    //}
    //if (pInput->IsStayKeyDown('4'))
    //{
    //    RotateWaist(m_WaistRotation.QUANTITY_FACTOR);
    //}
    ICamera*       pCurrentCamera = CurrentCamera()();
    InputManager*  pInput = Input()();
    Transform*     pTr = GetTransform();
    D3DXVECTOR3    p = pTr->GetPosition();
    D3DXQUATERNION r = pTr->GetRotation();
    const bool isPressing_LAlt = pInput->IsStayKeyDown(VK_LMENU);
    const bool isPressing_LCtrl  = pInput->IsStayKeyDown(VK_LCONTROL);
    const bool isPressing_LShift = pInput->IsStayKeyDown(VK_LSHIFT);
    const bool isPressing_W      = pInput->IsStayKeyDown('W');
    const bool isPressing_S      = pInput->IsStayKeyDown('S');
    const bool isPressing_A      = pInput->IsStayKeyDown('A');
    const bool isPressing_D      = pInput->IsStayKeyDown('D');

    const bool isPressed_Space = pInput->IsOnceKeyDown(VK_SPACE);
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
    if (isPressing_LAlt)
    {
        if (pCurrentCamera)
        {
            if (pCurrentCamera->GetTagCamera() == TAG_CAMERA::Third_Person)
            {

                //m_rotForCameraTP.x += -pitch;
                m_rotForCameraTP.y += yaw;
            }
        }
    }
    else // isPressing_LAlt == false
    {
        // update rotation of transform

        D3DXQUATERNION q;
        D3DXQuaternionRotationYawPitchRoll(&q, yaw, 0.0f, 0.0f);
        //cout << "pitch : " << pitch << " yaw : " << yaw << endl;

        r *= q;

        // reset rotFotCameraTP
        m_rotForCameraTP = Vector3::ZERO;
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

    Debug << "rot for TP : " << m_rotForCameraTP << '\n';

    switch (m_animState)
    {
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1:
        {
            if (isPressed_Space)
            {
                //if (isPressing_W)
                //{
                //    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_F, true);
                //}
                //else
                //{
                //    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Jump_Stationary, true);
                //}
            }
            else
            {
                if (isPressing_W)
                {
                    if (isPressing_LCtrl)
                    {
                        setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_F, true);
                        p += getForward() * 0.5f;
                    }
                    else if (isPressing_LShift)
                    {
                        setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F, true);
                        p += getForward() * 2.0f;
                    }
                    else
                    {
                        setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_F, true);
                        p += getForward();
                    }
                }
                else if (isPressing_S)
                {
                    if (isPressing_LCtrl)
                    {
                        setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_B, true);
                        p += getForward() * -0.5f;
                    }
                    else
                    {
                        setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B, true);
                        p += getForward() * -1.0f;
                    }
                }
            }
        }
        break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_F:
        {
            if (isPressing_W) 
            {
                p += getForward() * 0.5f;
            }
            else
            {
                setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
            }
        }
        break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Sprint_F:
        {
            if (isPressing_W) 
            {
                p += getForward() * 2.0f;
            }
            else
            {
                setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
            }
        }
        break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_F:
        {
            if (isPressing_W) 
            {
                p += getForward();
            }
            else
            {
                setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
            }
        }
        break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Walk_B:
        {
            if (isPressing_S)
            {
                p += getForward() * -0.5f;
            }
            else
            {
                setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
            }
        }
        break;
    case TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Run_B:
        {
            if (isPressing_S)
            {
                p += getForward() * -1.0f;
            }
            else
            {
                setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, true);
            }
        }
        break;
    }

    if (pInput->IsStayKeyDown('A'))
    {
        p += getRight() * -m_rootTransform.MOVE_SPEED;
        //isUpdated = true;
    }
    if (pInput->IsStayKeyDown('D'))
    {
        p += getRight() * m_rootTransform.MOVE_SPEED;
        //isUpdated = true;
    }

    //if (pInput->IsOnceKeyDown(VK_RETURN))
    //    isFired = true;

    pTr->SetPosition(p);
    pTr->SetRotation(r);

    Debug << "current animation : "
          << pSkinnedMeshController->GetCurrentAnimationName() << '\n';

    Debug << "current position : " << p << '\n';
}

void Character::updateOther()
{
    if (isMine()) return;

    auto pInput = Input()();
    auto pCom = Communication()();
    auto tr = GetTransform();
    auto pos = tr->GetPosition();
    auto isUpdated = false;
    auto isFired = false;
    auto pSkiCon = pSkinnedMeshController;

    auto& pi = pCom->m_RoomInfo.m_PlayerInfos[m_index];
    //pos = pi.m_Position;

    const auto uAnimState = static_cast<unsigned int>(m_animState);
    if (uAnimState != pi.m_AnimationIndex)
    {
        m_animState = static_cast<TAG_ANIM_CHARACTER>(pi.m_AnimationIndex);
        pSkiCon->SetAnimationIndex(pi.m_AnimationIndex, true);
    }

    tr->SetPosition(pos);
}