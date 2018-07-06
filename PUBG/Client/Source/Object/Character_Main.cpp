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
    , m_cellIndex(0)
    
    , m_rootTransform(1.0f)
    , m_waistRotation(D3DX_PI * 0.5f, 0.1f)
    , m_framePtr()
    , m_info()
    , m_savedInput()
    , m_currentStayKey()
    , m_totalInventory()
    , m_attacking(Attacking::Unarmed)
    , m_stance(Stance::Stand)
    , m_upperAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_lowerAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)

    , pAnimation(nullptr)
{
    const float factor(static_cast<float>(m_index + 1) * 100.0f);

    Transform* pTransform = GetTransform();
    pTransform->SetPosition(D3DXVECTOR3(factor, 0.0f, factor));
    pTransform->SetRotation(OFFSET_ROTATION);

    //putting character into TotalCellSpace
    IScene* CS = CurrentScene()();
    m_cellIndex = CS->GetCellIndex(pTransform->GetPosition());                   //ĳ������ pos�� ���� �˸��� area�� �־��ֱ�
    CS->InsertObjIntoTotalCellSpace(TAG_OBJECT::Character, m_cellIndex, this);   //Object �� TotalCellSpace(Area)�� �ֱ�
    CS->m_NearArea.Create(m_cellIndex);                                          //Near Area ���

    pAnimation = new CharacterAnimation;
    AddChild(pAnimation);
    pAnimation->Set(
        CharacterAnimation::BodyPart::BOTH, 
        TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1, 
        false);

    setFramePtr();

    // set boundingShapes
    m_characterParts.resize(static_cast<std::size_t>(TAG_COLLIDER_CHARACTER_PART::COUNT));
    AddPart(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::Head, this));

    for (auto pPart : m_characterParts)
        m_boundingBoxes.emplace_back(pPart->GetBoundingBox());

    auto a = pAnimation->GetBoundingSpheres();
    m_pBoundingSphere = new BoundingSphere;
    m_pBoundingSphere->center = a.front().center;
    m_pBoundingSphere->radius = a.front().radius;
    // end boundingShapes

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
    Transform* tm = GetTransform();

    //�̰����� Input�� �ֽ��ϴ� �׸��� m_currentStayKey���� ���
    handleInput(&m_currentStayKey);
    handleInput(&m_currentOnceKey);


    //m_currentState�� ������ ���� //dest pos �� �� ��
    State destState;
    destState.position = tm->GetPosition();
    destState.rotation = tm->GetRotation();
    destState.transformationMatrix = tm->GetTransformationMatrix();

    //*********************************


    movementControl(&destState);

    

    // �浹üũ////////////////////////////
    bool hasCollision = false;
    BoundingSphere pMyBoundingSphere = GetBoundingSphere()->CopyTo(destState.position);
    for (auto tf : pCurrentScene->m_NearArea.GetTerrainFeatures())
    {
        if (hasCollision) break;

        // �ٿ�����Ǿ �浹���� ������ ���� �ͷ�����ó�� �浹�� �˻��Ѵ�.
        if (!Collision::HasCollision(pMyBoundingSphere, *(tf->GetBoundingSphere()))) continue;

        if (destState.boundingBoxes.empty())
        {
            D3DXMatrixTransformation(&destState.transformationMatrix, nullptr, nullptr, nullptr, nullptr, &destState.rotation, &destState.position);

            for (auto bb : GetBoundingBoxes())
            {
                destState.boundingBoxes.emplace_back(bb->CopyTo(destState.transformationMatrix));
            }
        }

        for (auto& mine : destState.boundingBoxes)
        {
            if (hasCollision) break;

            for (auto& others : tf->GetBoundingBoxes())
            {
                if (hasCollision) break;

                hasCollision = Collision::HasCollision(mine, *others);
            }
        }
    }
    // end collision /////////////////////////

    // �� Ŀ��Ʈ
    if (hasCollision)
    {
        // ���� ������Ʈ�� ��쿡�� ����ƮŰ ������ ������� ������ �ٲ۴�.
        if (destState.isHeadBump)
        {
            // TODO : impl
        }
    }
    else
    {
        // update state
        tm->SetPosition(destState.position);
        tm->SetRotation(destState.rotation);

        // �̻��ϱ� //NearArea(cell space)�� �ٽ� ���ϱ�!
        std::size_t destCellIndex = pCurrentScene->GetCellIndex(destState.position);
        if (destCellIndex != m_cellIndex)
        {
            pCurrentScene->m_NearArea.Create(destCellIndex);
            pCurrentScene->MoveCell(&m_cellIndex, destCellIndex, TAG_OBJECT::Character, this);
        }
    }

    setStance();
    setAttacking();
    setReload();
    setPunch();
    setInteraction();
    setJump();
    animationControl();

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

    // ī�޶� �������� ������Ʈ (�ֳ��ϸ� ĳ����0 ������Ʈ, ����, ĳ����1 ������Ʈ, ����, ... ������� ����Ǳ� ������)
    CurrentCamera()()->UpdateFrustumCulling();
    
    D3DXVECTOR3 pos = tm->GetPosition();
    D3DXQUATERNION rot = tm->GetRotation();

    cameraCharacterRotation(dt, &rot);//�ɸ��Ϳ� ī�޶��� rotation�� ����ؼ� �ְ� �ȴ�.
    applyTarget_Y_Position(&pos); //apply height and control jumping
    tm->SetPosition(pos);
    tm->SetRotation(rot);


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





///////////////////////////////////////////////////////////////////////////////

    Sound()()->Listen(pos, getForward());

    if (Input()()->IsOnceKeyDown(VK_UP))
    {
        BulletPool()()->Fire(pos + getForward() * 100.0f, getBackward(), 0.1f, 1.0f, TAG_COLLISION::Player_1_Damage);

    }

    ForDebug();
    Debug << "current        position : " << pos << "\n\n"
        << "current upper animation : "
        << pAnimation->GetUpperAnimationName() << '\n'
        << "current lower animation : "
        << pAnimation->GetLowerAnimationName() << "\n\n";

    //Communication()()->SendPosition(pos);
}

void Character::updateOther()
{
    if (isMine()) return;

    auto pInput = Input()();
    auto pCom = Communication()();
    D3DXVECTOR3 pos = GetTransform()->GetPosition();

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