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
    , m_cellSpaceIndex(0)
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
    
    m_cellSpaceIndex = CurrentScene()()->GetCellIndex(pTransform->GetPosition());
    CurrentScene()()->InsertObjIntoCellSpace(TAG_OBJECT::Character, m_cellSpaceIndex, this);
    
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
    D3DXVECTOR3    scl = pTransform->GetScale();
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

    //save pos�� ������ ����
    //dest pos �� �� ��

    setStance();
    setAttacking();
    setReload();
    setPunch();
    setInteraction();
    setJump();

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
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH, 
                m_lowerAnimState);

            // set current with same position with finish event m_lowerAnimState

            m_savedInput = m_currentStayKey;
        }
    }
    //else
    //{
    //    animationMovementControl(&pos, NULL); // NULL means not changing animation
    //}

    //dest pos,rot ����

    //dest�� ���� ������Ʈ 
    // ī�޶� �������� ������Ʈ 
    CurrentCamera()()->UpdateFrustumCulling();
    //dest�� ������Ʈ�� frustum�� min max �簢���� �����Ѵ�(�Լ�) >> x z float �� 4���� ���ͼ� area ����
    //(������ �簢������ �Ѽ��� �� ũ��)


    //bounding sphere(destpos + center, radius)�� ���ؼ�(���ο� ������ �־���´�)

    //for ��
    //area ���� �ִ� ��� terrain feature ��  ���� bounding sphere�� collision�� �ִ°��� ã�Ƴ���
    /*{
    
        
        //if has sphere collision 
        {
            dest tm�� ���, dest bounding box ���

            this��(ĳ������ box colliders ��) �� terrain feature �� ���� �ִ� box collider ���� ��� 
            if(�ϳ��� �浹�ϸ� ���� ���´� -> �÷��̾ �������� ���ϵ���)

            else �ϳ��� �浹���� �ʴ´ٸ�
            this�� ������Ʈ pos = destpos
            
        }

    }*/
    
    // character update ����



    // �̵�������ġ üũ�ؼ� ������Ʈ �ϰų� y���ǰų� ���
    //CurrentScene()()->Get
    IScene* pCurrScene = CurrentScene()();
    if (pCurrScene->IsMovable(&pos, m_cellSpaceIndex, TAG_OBJECT::Character, this))
    {
        //�̰��� �ؾ� �Ѵ�
    }
    
    // �̵������ϴٸ� cellIndex = CurrentScene()()->GetCellIndex(pos);
    // if (m_cellIndex != cellIndex) �̻��ϱ�
    size_t nextCellSpace = pCurrScene->GetCellIndex(pos);
    if (m_cellSpaceIndex != nextCellSpace)
    {
        pCurrScene->MoveCell(&m_cellSpaceIndex, nextCellSpace,TAG_OBJECT::Character,this);
    }

    cameraCharacterRotation(dt, &rot);//�ɸ��Ϳ� ī�޶��� rotation�� ����ؼ� �ְ� �ȴ�.
    applyTarget_Y_Position(&pos); //apply height and control jumping
    //�ɸ��Ϳ� ī�޶��� rotation�� ����ؼ� �ְ� �ȴ�.
    

    m_totalInventory.m_bulletFireCoolDown -= dt;
    if (m_totalInventory.m_bulletFireCoolDown <= 0.f) m_totalInventory.m_bulletFireCoolDown = 0.f;
    if (m_attacking == Attacking::Rifle && m_currentOnceKey._LButton)
    {
        if (m_totalInventory.m_bulletFireCoolDown <= 0.f &&  m_totalInventory.m_pHand->GetNumBullet() > 0)
        {
            rifleShooting();
            //pistolShooting();?? �̶��͵� ���߿��� ���������?
        }
    }

    Sound()()->Listen(pos, getForward());

    if (Input()()->IsOnceKeyDown(VK_UP))
    {
        BulletPool()()->Fire(pos + getForward() * 100.0f, getBackward(), 0.1f, 1.0f, TAG_COLLISION::Player_1_Damage);

    }

    Debug << "current        position : " << pos << "\n\n"
        << "current upper animation : "
        << pAnimation->GetUpperAnimationName() << '\n'
        << "current lower animation : "
        << pAnimation->GetLowerAnimationName() << "\n\n";

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
    //    pAnimation->SetAnimationIndex(pi.m_AnimationIndex, true);
    //}

    //tr->SetPosition(pos);
}