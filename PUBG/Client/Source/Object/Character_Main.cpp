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

    //이곳에서 Input을 넣습니다 그리고 m_currentStayKey으로 사용
    handleInput(&m_currentStayKey);
    handleInput(&m_currentOnceKey);

    //save pos를 저장해 놓고
    //dest pos 로 계 산

    setStance();
    setAttacking();
    setReload();
    setPunch();
    setInteraction();
    setJump();

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

    //dest pos,rot 결정

    //dest를 갖고 업데이트 
    // 카메라 프러스텀 업데이트 
    CurrentCamera()()->UpdateFrustumCulling();
    //dest로 업데이트된 frustum의 min max 사각형을 결정한다(함수) >> x z float 총 4개가 나와서 area 결정
    //(결정된 사각형에서 한셀씩 더 크게)


    //bounding sphere(destpos + center, radius)를 구해서(새로운 변수에 넣어놓는다)

    //for 문
    //area 내에 있는 모든 terrain feature 와  내꺼 bounding sphere랑 collision이 있는것을 찾아낸다
    /*{
    
        
        //if has sphere collision 
        {
            dest tm을 계산, dest bounding box 계산

            this의(캐릭터의 box colliders 들) 과 terrain feature 가 갖고 있는 box collider 들을 계산 
            if(하나라도 충돌하면 빠져 나온다 -> 플레이어가 움직이지 못하도록)

            else 하나라도 충돌하지 않는다면
            this를 업데이트 pos = destpos
            
        }

    }*/
    
    // character update 끝남



    // 이동가능한치 체크해서 업데이트 하거나 y만되거나 등등
    //CurrentScene()()->Get
    IScene* pCurrScene = CurrentScene()();
    if (pCurrScene->IsMovable(&pos, m_cellSpaceIndex, TAG_OBJECT::Character, this))
    {
        //이곳을 해야 한다
    }
    
    // 이동가능하다면 cellIndex = CurrentScene()()->GetCellIndex(pos);
    // if (m_cellIndex != cellIndex) 이사하기
    size_t nextCellSpace = pCurrScene->GetCellIndex(pos);
    if (m_cellSpaceIndex != nextCellSpace)
    {
        pCurrScene->MoveCell(&m_cellSpaceIndex, nextCellSpace,TAG_OBJECT::Character,this);
    }

    cameraCharacterRotation(dt, &rot);//케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    applyTarget_Y_Position(&pos); //apply height and control jumping
    //케릭터와 카메라의 rotation을 계산해서 넣게 된다.
    

    m_totalInventory.m_bulletFireCoolDown -= dt;
    if (m_totalInventory.m_bulletFireCoolDown <= 0.f) m_totalInventory.m_bulletFireCoolDown = 0.f;
    if (m_attacking == Attacking::Rifle && m_currentOnceKey._LButton)
    {
        if (m_totalInventory.m_bulletFireCoolDown <= 0.f &&  m_totalInventory.m_pHand->GetNumBullet() > 0)
        {
            rifleShooting();
            //pistolShooting();?? 이란것도 나중에는 만들겠지요?
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