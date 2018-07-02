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
    , m_info()
    , m_savedInput()
    , m_currentInput()
    , m_pSphereMesh(nullptr)
    , m_pRootCharacterPart(nullptr)
    , m_totalInventory()

    , pSkinnedMeshController(nullptr)
{
    Transform* tr = GetTransform();
    tr->SetRotation(OFFSET_ROTATION);
    tr->SetPosition(D3DXVECTOR3(static_cast<float>(index) * 100.0f, 0.0f, 0.0f));

    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
    pSkinnedMeshController->SetSkinnedMesh(
        Resource()()->GetCharacterSkinnedMesh());
    
    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false);

    //for test
    //setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_PrimarySlot_OnHand, false);
    //setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_SecondarySlot_OnHand, false);

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
    //// for distance measurement
    for (int i = 0; i < 10; ++i)
    {
        D3DXMATRIX t;
        D3DXMatrixTranslation(&t, static_cast<float>(i * 10), 0.0f, 0.0f);

        Shader::Draw(
            Resource()()->GetEffect("./Resource/", "Color.fx"),
            nullptr,
            m_pSphereMesh,
            0,
            [this, &t](LPD3DXEFFECT pEffect)
        {
            pEffect->SetMatrix(Shader::World, &t);

            D3DXCOLOR white(1.0f, 1.0f, 1.0f, 1.0f);
            pEffect->SetValue("Color", &white, sizeof white);
        });
    }
}

void Character::updateMine()
{
    if (!isMine()) return;

    //// for anim debug
    //InputManager*  pInput = Input()();
    //auto animIndex = pSkinnedMeshController->GetCurrentAnimationIndex();
    //const auto numAnim = pSkinnedMeshController->GetNumAnimation();

    //if (pInput->IsOnceKeyDown('1'))
    //{
    //    if (animIndex > 0)
    //        animIndex--;

    //    pSkinnedMeshController->SetAnimationIndex(animIndex, true);
    //}
    //if (pInput->IsOnceKeyDown('2'))
    //{
    //    if (animIndex < numAnim - 1)
    //        animIndex++;

    //    pSkinnedMeshController->SetAnimationIndex(animIndex, true);
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



    Transform*     pTr = GetTransform();
    D3DXVECTOR3    p = pTr->GetPosition();
    D3DXQUATERNION r = pTr->GetRotation();


    /****************여러분! delta time 을 넣을 까요???*************/

    //이곳에서 Input을 넣습니다 그리고 m_currentInput으로 사용
    m_currentInput = HandleInput(m_currentInput);

    if (m_savedInput != m_currentInput)
    {
        //setting animation and movements
        AnimationMovementControl(&p, &m_animState);
        if (m_animState == TAG_ANIM_CHARACTER::COUNT)
        {

        }
        else
        {
            setAnimation(m_animState, true);

            m_savedInput = m_currentInput;
        }
    }
    else
    {
        AnimationMovementControl(&p, NULL); // NULL means not changing animation
    }


    //케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    CameraCharacterRotation(&r);
    //animation Switch 문





    //if (pInput->IsOnceKeyDown(VK_RETURN))
    //    isFired = true;

    pTr->SetPosition(p);
    pTr->SetRotation(r);

    Debug << "current animation : "
          << pSkinnedMeshController->GetCurrentAnimationName() << '\n';

    Debug << "current position : " << p << '\n';


    //sh tset
    if (Input()()->IsOnceKeyDown('B'))
    {
        Sound()()->SetPosition(D3DXVECTOR3(0, 0, 100));
        Sound()()->SetVolume(0.8f);
        Sound()()->Play(TAG_SOUND::Kar98_NormalShoot);
    }
    
    if (Input()()->IsOnceKeyDown('N'))
    {
        Sound()()->Play(TAG_SOUND::Kar98_BoltMove0, (D3DXVECTOR3(0, 0, -100)),
            0.5f, FMOD_3D);
    }

    Sound()()->Listen(GetTransform()->GetPosition(), getForward());
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