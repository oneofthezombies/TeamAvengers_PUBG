#include "stdafx.h"
#include "Character.h"
#include "CharacterPart.h"
#include "Bullet.h"
#include "SkinnedMeshController.h"
#include "DirectionalLight.h"
#include "AnimationState.h"
#include "ResourceInfo.h"

const D3DXQUATERNION Character::OFFSET_ROTATION = 
    D3DXQUATERNION(0.0f, 1.0f, 0.0f, 0.0f);

Character::Character(const int index)
    : IObject()

    , m_index(index)
    , m_animState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_rootTransform(1.0f)
    , m_waistRotation(D3DX_PI * 0.5f, 0.1f)
    , m_framePtr()
    , m_info()
    , m_savedInput()
    , m_currentStayKey()
    , m_pSphereMesh(nullptr)
    , m_pRootCharacterPart(nullptr)
    , m_totalInventory()
    , m_attacking(Attacking::Unarmed)
    , m_stance(Stance::Stand)
    , m_pLowerBodyController(nullptr)

    , pUpperBodyController(nullptr)
{
    Transform* tr = GetTransform();
    tr->SetRotation(OFFSET_ROTATION);
    tr->SetPosition(
        D3DXVECTOR3(static_cast<float>(index) * 100.0f, 0.0f, 0.0f));

    const auto pathFilename = 
        ResourceInfo::GetCharacterPathFileName();

    pUpperBodyController = AddComponent<SkinnedMeshController>();
    pUpperBodyController->SetSkinnedMesh(
        Resource()()->GetSkinnedMesh(pathFilename.first, pathFilename.second));

    m_pLowerBodyController = new SkinnedMeshController(this);
    m_pLowerBodyController->SetSkinnedMesh(
        Resource()()->GetSkinnedMesh(pathFilename.first, pathFilename.second));
    
    setAnimation(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, false);
    //setAnimation(TAG_ANIM_CHARACTER::Rifle_Combat_Stand_Base_LocoIdle, false);

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


    const float    dt = Time()()->GetDeltaTime();
    Transform*     pTr = GetTransform();
    D3DXVECTOR3    p = pTr->GetPosition();
    D3DXQUATERNION r = pTr->GetRotation();

    //이곳에서 Input을 넣습니다 그리고 m_currentStayKey으로 사용
    handleInput(&m_currentStayKey);
    handleInput(&m_currentOnceKey);

    setStance();
    setAttacking();
    setReload();

    // TODO : 앉아있을 때 점프(스페이스) -> 일어섬
    if (m_savedInput != m_currentStayKey)
    {
        //setting animation and movements
        animationMovementControl(&p, &m_animState);
        if (m_animState == TAG_ANIM_CHARACTER::COUNT)
        {
            //애니메이션이 없습니다.
        }
        else
        {
            setAnimation(m_animState);

            m_savedInput = m_currentStayKey;
        }
    }
    else
    {
        animationMovementControl(&p, NULL); // NULL means not changing animation
    }

    cameraCharacterRotation(dt, &r);//케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    applyTarget_Y_Position(&p); //apply height and control jumping
    //케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    
    pTr->SetPosition(p);
    pTr->SetRotation(r);

    Debug << "current animation : "
          << pSkinnedMeshController->GetCurrentAnimationName() << '\n';




    //sh tset
    if (Input()()->IsOnceKeyDown('B'))
    {
        Sound()()->SetPosition(D3DXVECTOR3(0, 0, 100));
        Sound()()->SetVolume(0.8f);
        Sound()()->Play(TAG_SOUND::Kar98_NormalShoot);
    }
    
    if (Input()()->IsOnceKeyDown('N'))
    {
        //Sound()()->Play(TAG_SOUND::Kar98_BoltMove0, (D3DXVECTOR3(0, 0, -100)), 0.5f, FMOD_3D);
        Communication()()->SendEventSound(TAG_SOUND::Kar98_NormalShoot, (D3DXVECTOR3(0, 0, -100)));
    }

    Sound()()->Listen(GetTransform()->GetPosition(), getForward());

    //Communication()()->SendPosition(p);
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
    pos = pi.m_Position;

    const auto uAnimState = static_cast<unsigned int>(m_animState);
    if (uAnimState != pi.m_AnimationIndex)
    {
        m_animState = static_cast<TAG_ANIM_CHARACTER>(pi.m_AnimationIndex);
        pSkiCon->SetAnimationIndex(pi.m_AnimationIndex, true);
    }

    tr->SetPosition(pos);
}