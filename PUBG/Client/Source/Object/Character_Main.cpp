#include "stdafx.h"
#include "Character.h"
#include "CharacterAnimation.h"
#include "CharacterPart.h"
#include "Bullet.h"
#include "DirectionalLight.h"
#include "AnimationState.h"
#include "ResourceInfo.h"

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
    , m_pAnimation(nullptr)
    , m_pRootCharacterPart(nullptr)
{
    pTransform = GetTransform();
    pTransform->SetRotation(OFFSET_ROTATION);
    pTransform->SetPosition(D3DXVECTOR3(100.0f, 0.0f, 100.0f));

    m_pAnimation = new CharacterAnimation;
    m_pAnimation->Set(
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
    SAFE_DELETE(m_pAnimation);
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

    //�̰����� Input�� �ֽ��ϴ� �׸��� m_currentStayKey���� ���
    handleInput(&m_currentStayKey);
    handleInput(&m_currentOnceKey);

    setStance();
    setAttacking();
    setReload();
    ForDebug();

    //setting animation and movements
    animationMovementControl(&pos, &m_lowerAnimState);

    // TODO : �ɾ����� �� ����(�����̽�) -> �Ͼ
    if (m_savedInput != m_currentStayKey)
    {
        if (m_lowerAnimState == TAG_ANIM_CHARACTER::COUNT)
        {
            //�ִϸ��̼��� �����ϴ�.
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

    cameraCharacterRotation(dt, &rot);//�ɸ��Ϳ� ī�޶��� rotation�� ����ؼ� �ְ� �ȴ�.
    applyTarget_Y_Position(&pos); //apply height and control jumping
    //�ɸ��Ϳ� ī�޶��� rotation�� ����ؼ� �ְ� �ȴ�.
    
    //John TODO list
    //<<1. CalcPickedPosition �Լ��� v�� �����ͷ� �ް� �ϱ�

    if (Input()()->IsOnceKeyDown(VK_LBUTTON))
    {
        //������ ��� �ٲܲ��ĸ� ���� r�� ��� targetpos - �ѱ� pos �ؼ� ������ direction���� ��� �ɷ� �Ҳ���!
        D3DXVECTOR3 v; 
        CurrentCamera()()->CalcPickedPosition(v, 1207 / 2, 600 / 2); 
        D3DXVECTOR3 dir = v - (/*�̰��� ���߿� �ѱ� pos*/pos + D3DXVECTOR3(0, 100.0f, 0));
        
        D3DXQUATERNION rrr;
        D3DXQuaternionRotationYawPitchRoll(&rrr, m_rotationForCamera.y, m_rotationForCamera.x, m_rotationForCamera.z);
        BulletPool()()->Fire((pos + D3DXVECTOR3(0, 100.0f, 0)), rrr * Character::OFFSET_ROTATION, 1.0f, 10.0f, TAG_COLLISION::Impassable);
            
            
            
        //���డ��    
        //BulletPool()()->Fire((p + D3DXVECTOR3(0,100.0f,0)), r*Character::OFFSET_ROTATION, 1.0f, 10.0f, TAG_COLLISION::Impassable);
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
        Communication()()->SendEventSound(TAG_SOUND::Kar98_NormalShoot, (D3DXVECTOR3(0, 0, -100)));
    }

    Sound()()->Listen(GetTransform()->GetPosition(), getForward());

    Debug << "current        position : " << pos << "\n\n"
        << "current upper animation : "
        << m_pAnimation->GetUpperAnimationName() << '\n'
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

    //auto pInput = Input()();
    //auto pCom = Communication()();
    //auto pos = pTransform->GetPosition();
    //auto pSkiCon = pSkinnedMeshController;

    //auto& pi = pCom->m_RoomInfo.m_PlayerInfos[m_index];
    //pos = pi.m_Position;

    //const auto uAnimState = static_cast<unsigned int>(m_animState);
    //if (uAnimState != pi.m_AnimationIndex)
    //{
    //    m_animState = static_cast<TAG_ANIM_CHARACTER>(pi.m_AnimationIndex);
    //    pSkiCon->SetAnimationIndex(pi.m_AnimationIndex, true);
    //}

    //tr->SetPosition(pos);
}