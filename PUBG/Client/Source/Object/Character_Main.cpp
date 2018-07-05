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
#include "TerrainFeature.h"

const D3DXQUATERNION Character::OFFSET_ROTATION = 
    D3DXQUATERNION(0.0f, 1.0f, 0.0f, 0.0f);

Character::Character(const int index)
    : IObject(TAG_OBJECT::Character)

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
    //, m_pRootCharacterPart(nullptr)
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

    m_characterParts.resize(static_cast<std::size_t>(TAG_COLLIDER_CHARACTER_PART::COUNT));
    AddPart(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::Head, this));

    for (auto pPart : m_characterParts)
        m_boundingBoxes.emplace_back(pPart->GetBoundingBox());

    subscribeCollisionEvent();

    if (isMine())
    {
        setInfo();
        Camera()()->SetTarget(&m_info);
        m_rotationForCamera = Vector3::ZERO;
    }

    auto a = pAnimation->GetBoundingSpheres();
    m_pBoundingSphere = new BoundingSphere;
    m_pBoundingSphere->center = a.front().center;
    m_pBoundingSphere->radius = a.front().radius;
}

Character::~Character()
{
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
    IScene*        pCurrentScene = CurrentScene()();
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
    State dest; // = current

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
            pAnimation->Set(
                CharacterAnimation::BodyPart::BOTH, 
                m_lowerAnimState);

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
    //CurrentCamera()()->UpdateFrustumCulling();


    /////////////////// 
    dest.cellIndex =  CurrentScene()()->GetCellIndex(dest.position);
    if (dest.cellIndex != m_cellSpaceIndex)
        pCurrentScene->m_NearArea.Create(dest.cellIndex);
    ////////////////////////////

    bool hasCollision = false;
    BoundingSphere pMyBoundingSphere = GetBoundingSphere()->CopyTo(dest.position);
    for (auto tf : pCurrentScene->m_NearArea.GetTerrainFeatures())
    {
        if (hasCollision) break;

        // �ٿ�����Ǿ �浹���� ������ ���� �ͷ�����ó�� �浹�� �˻��Ѵ�.
        if (!Collision::HasCollision(pMyBoundingSphere, *(tf->GetBoundingSphere()))) continue;

        if (dest.boundingBoxes.empty())
        {
            D3DXMatrixTransformation(&dest.transformationMatrix, nullptr, nullptr, nullptr, nullptr, &dest.rotation, &dest.position);

            for (auto bb : GetBoundingBoxes())
            {
                dest.boundingBoxes.emplace_back(bb->CopyTo(dest.transformationMatrix));
            }
        }

        for (auto& mine : dest.boundingBoxes)
        {
            if (hasCollision) break;

            for (auto& others : tf->GetBoundingBoxes())
            {
                if (hasCollision) break;

                hasCollision = Collision::HasCollision(mine, *others);
            }
        }
    }

    if (!hasCollision)
    {
        // update state
    }

    
    //CurrentScene()()->m_NearArea.GetNearArea();



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
        if (m_totalInventory.m_bulletFireCoolDown <= 0.f &&  m_totalInventory.m_hand->GetNumBullet() > 0)
        {
            rifleShooting();
            //pistolShooting();?? �̶��͵� ���߿��� ���������?
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