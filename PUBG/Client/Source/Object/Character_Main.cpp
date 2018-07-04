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

const D3DXQUATERNION Character::OFFSET_ROTATION = 
    D3DXQUATERNION(0.0f, 1.0f, 0.0f, 0.0f);

Character::Character(const int index)
    : IObject()

    // id
    , m_index(index)

    , m_upperAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_lowerAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_rootTransform(1.0f)
    , m_waistRotation(D3DX_PI * 0.5f, 0.1f)
    , m_framePtr()
    , m_info()
    , m_savedInput()
    , m_currentStayKey()
    , m_totalInventory()
    , m_attacking(Attacking::Unarmed)
    , m_stance(Stance::Stand)

    , pTransform(nullptr)
    , pAnimation(nullptr)
    , m_pRootCharacterPart(nullptr)
{
    pTransform = GetTransform();
    pTransform->SetRotation(OFFSET_ROTATION);
    const float factor(static_cast<float>(m_index + 1) * 100.0f);
    pTransform->SetPosition(D3DXVECTOR3(factor, 0.0f, factor));

    pAnimation = new CharacterAnimation;
    AddChild(pAnimation);
    pAnimation->Set(
        CharacterAnimation::BodyPart::BOTH, 
        TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, 
        false);

    setFramePtr();

    m_pRootCharacterPart =
        new CharacterPart(TAG_COLLIDER_CHARACTER_PART::Head, this);

    subscribeCollisionEvent();

    if (isMine())
    {
        setInfo();
        Camera()()->SetTarget(&m_info);
        m_rotationForCamera = Vector3::ZERO;
    }
}

Character::~Character()
{
    SAFE_DELETE(m_pRootCharacterPart);
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
}

void Character::updateMine()
{
    if (!isMine()) return;

    const float    dt = Time()()->GetDeltaTime();
    D3DXVECTOR3    pos = pTransform->GetPosition();
    D3DXQUATERNION rot = pTransform->GetRotation();

    //if (pInput->IsStayKeyDown('3'))
    //{
    //    RotateWaist(-m_WaistRotation.QUANTITY_FACTOR);
    //}
    //if (pInput->IsStayKeyDown('4'))
    //{
    //    RotateWaist(m_WaistRotation.QUANTITY_FACTOR);
    //}

    //이곳에서 Input을 넣습니다 그리고 m_currentStayKey으로 사용
    handleInput(&m_currentStayKey);
    handleInput(&m_currentOnceKey);

    setStance();
    setAttacking();
    setReload();
    ForDebug();

    //setting animation and movements
    animationMovementControl(&pos, &m_lowerAnimState);

    // TODO : 앉아있을 때 점프(스페이스) -> 일어섬
    if (m_savedInput != m_currentStayKey)
    {
        if (m_lowerAnimState == TAG_ANIM_CHARACTER::COUNT)
        {
            //애니메이션이 없습니다.
        }
        else
        {
            m_pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH, 
                m_lowerAnimState);

            m_savedInput = m_currentStayKey;
        }
    }
    //else
    //{
    //    animationMovementControl(&pos, NULL); // NULL means not changing animation
    //}

    cameraCharacterRotation(dt, &rot);//케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    applyTarget_Y_Position(&pos); //apply height and control jumping
    //케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    

    m_totalInventory.m_bulletFireCoolDown -= dt;
    if (m_totalInventory.m_bulletFireCoolDown <= 0.f) m_totalInventory.m_bulletFireCoolDown = 0.f;
    if (m_attacking == Attacking::Rifle && m_currentOnceKey._LButton)
    {
        if (m_totalInventory.m_bulletFireCoolDown <= 0.f &&  m_totalInventory.m_hand->GetNumBullet() > 0)
        {
            rifleShooting();
            //pistolShooting();?? 이란것도 나중에는 만들겠지요?
        }
    }

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
        //Communication()()->SendEventSound(TAG_SOUND::Kar98_NormalShoot, (D3DXVECTOR3(0, 0, -100)));
        Sound()()->addPlay(TAG_SOUND::Kar98_NormalShoot, (D3DXVECTOR3(0, 0, -100)), 5.0f, FMOD_3D);
    }
    
    //float test_distance = 777;
    //float ddd = test_distance / 340;
    //
    //float lastTime_2 = Time()()->GetDeltaTime();
    //float ingTime = Time()()->GetDeltaTime() - lastTime_2;
    //if (ddd <= ingTime)
    //{
    //    Sound()()->Play(TAG_SOUND::Kar98_BoltMove0, (D3DXVECTOR3(0, 0, -100)), 0.5f, FMOD_3D);
    //}

    Sound()()->Listen(pos, getForward());

    if (Input()()->IsOnceKeyDown(VK_UP))
    {
        BulletPool()()->Fire(pos + getForward() * 100.0f, getBackward(), 0.1f, 1.0f, TAG_COLLISION::Player_1_Damage);

    }

    Debug << "current        position : " << pos << "\n\n"
        << "current upper animation : "
        << pAnimation->GetUpperAnimationName() << '\n'
        << "current lower animation : "
        << m_pAnimation->GetLowerAnimationName() << "\n\n";

///////////////////////////////////////////////////////////////////////////////

    // set transform
    pTransform->SetPosition(pos);
    pTransform->SetRotation(rot);

    //Communication()()->SendPosition(pos);
}

void Character::updateOther()
{
    if (isMine()) return;

    auto pInput = Input()();
    auto pCom = Communication()();
    D3DXVECTOR3 pos = pTransform->GetPosition();

    auto& pi = pCom->m_RoomInfo.m_PlayerInfos[m_index];
    pos = pi.m_position;

    //const auto uAnimState = static_cast<unsigned int>(m_animState);
    //if (uAnimState != pi.m_AnimationIndex)
    //{
    //    m_animState = static_cast<TAG_ANIM_CHARACTER>(pi.m_AnimationIndex);
    //    m_pAnimation->SetAnimationIndex(pi.m_AnimationIndex, true);
    //}

    //tr->SetPosition(pos);
}