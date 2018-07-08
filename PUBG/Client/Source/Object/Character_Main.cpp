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
    , m_health(100.0f)
    , m_mouseInput()
    , m_rootTransform(1.0f)
    , m_waistRotation(0.8f, 0.1f)
    , m_headRotation(0.8f, 0.1f)

    , m_framePtr()
    , m_info()
    , m_savedInput()
    , m_currentStayKey()
    , m_totalInventory()
    , m_attacking(Attacking::Unarmed)
    , m_stance(Stance::Stand)
    , m_moving(Moving::Run)
    , m_direction(Direction::Front)
    , m_upperAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_lowerAnimState(TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1)
    , m_isFire(false)

    , pAnimation(nullptr)

    , m_otherHitPart(0)

{
    const float factor(static_cast<float>(m_index + 1) * 100.0f);

    Transform* pTransform = GetTransform();
    pTransform->SetPosition(D3DXVECTOR3(factor, 200.0f, factor));
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
        TAG_ANIM_CHARACTER::Unarmed_Combat_Stand_Idling_1);

    setFramePtr();

    // set boundingShapes
    m_characterParts.resize(static_cast<std::size_t>(TAG_COLLIDER_CHARACTER_PART::COUNT));
    AddPart(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::Head, this));

    for (auto pPart : m_characterParts)
        m_boundingBoxes.emplace_back(pPart->GetBoundingBoxes().front());

    const std::vector<BoundingSphere>& bss = pAnimation->GetBoundingSpheres();
    m_boundingSphere = bss.front();
    m_boundingSphere.position = pTransform->GetPosition();
    // end boundingShapes

    subscribeCollisionEvent();

    if (isMine())
    {
        setInfo();
        Camera()()->SetTarget(&m_info);
        m_rotationForCamera = Vector3::ZERO;
    }

    pOtherHitPositionMesh = Resource()()->GetBoundingSphereMesh();
}

Character::~Character()
{
    for (auto p : m_characterParts)
    {
        SAFE_DELETE(p);
    }
}

void Character::OnUpdate()
{
    updateMine();
    updateOther();

    // update
    GetTransform()->Update();
    pAnimation->UpdateAnimation();
    updateBone();
    pAnimation->UpdateModel();
    updateTotalInventory();

    // bounding sphere move to character position
    m_boundingSphere.position = GetTransform()->GetPosition();

    for (auto pPart : m_characterParts)
        pPart->Update();

    if (IsFire())
        RifleShooting();

    // render
    pAnimation->Render(
        /*m_framePtr.pWaist->CombinedTransformationMatrix
        **/ GetTransform()->GetTransformationMatrix(),
        [this](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(
            Shader::World,
            &GetTransform()->GetTransformationMatrix());

        DirectionalLight* light = CurrentScene()()->GetDirectionalLight();
        D3DXVECTOR3 lightDir = light->GetDirection();
        pEffect->SetValue(Shader::lightDirection, &lightDir, sizeof lightDir);
    });
    renderTotalInventory();

    for (auto pPart : m_characterParts)
        pPart->Render();

    m_boundingSphere.Render();

    //Shader::Draw(
    //    Resource()()->GetEffect("./Resource/", "Color.fx"), 
    //    nullptr, 
    //    pOtherHitPositionMesh, 
    //    0,
    //    [this](LPD3DXEFFECT pEffect)
    //{
    //    const D3DXVECTOR3& pos = m_otherHitPosition;
    //    D3DXMATRIX s, t, m;
    //    const float scale = 10.0f;
    //    D3DXMatrixScaling(&s, scale, scale, scale);
    //    D3DXMatrixTranslation(&t, pos.x, pos.y, pos.z);
    //    m = s * t;

    //    pEffect->SetMatrix(Shader::World, &m);
    //});

    auto& b = m_otherHitBox;
    D3DXMATRIX e, c, r, p, m;
    D3DXMatrixScaling(&e, b.extent.x, b.extent.y, b.extent.z);
    D3DXMatrixTranslation(&c, b.center.x, b.center.y, b.center.z);
    D3DXMatrixRotationQuaternion(&r, &b.rotation);
    D3DXMatrixTranslation(&p, b.position.x, b.position.y, b.position.z);
    m = e * c * r * p;
    const auto& vertices = Resource()()->GetBoundingBoxVertices();
    const auto& indices = Resource()()->GetBoundingBoxIndices();
    Shader::Draw(Resource()()->GetEffect("./Resource/", "Color.fx"), nullptr, 
        [&m](LPD3DXEFFECT pEffect)
    {

        pEffect->SetMatrix(Shader::World, &m);
        D3DXCOLOR magenta(1.0f, 0.0f, 1.0f, 1.0f);
        pEffect->SetValue("Color", &magenta, sizeof magenta);
    }, [&vertices, &indices]()
    {
        Device()()->DrawIndexedPrimitiveUP(
            D3DPT_LINELIST,
            0,
            vertices.size(),
            indices.size() / 2,
            indices.data(),
            D3DFMT_INDEX16,
            vertices.data(),
            sizeof vertices.front());
    });
    
    // communication
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
    handleMouse(dt, &m_mouseInput);


    //m_currentState�� ������ ���� //dest pos �� �� ��
    State destState;
    destState.position = tm->GetPosition();
    destState.rotation = tm->GetRotation();

    //*********************************


    movementControl(&destState);

    

    // �浹üũ////////////////////////////
    bool hasCollision = false;
    for (auto tf : pCurrentScene->m_NearArea.GetTerrainFeatures())
    {
        if (hasCollision) break;

        // �ٿ�����Ǿ �浹���� ������ ���� �ͷ�����ó�� �浹�� �˻��Ѵ�.
        if (!Collision::HasCollision(m_boundingSphere, tf->GetBoundingSphere())) continue;

        if (destState.boundingBoxes.empty())
        {
            for (auto& bb : GetBoundingBoxes())
            {
                BoundingBox destBB = bb;
                destBB.rotation = destState.rotation;
                destBB.position = destState.position;
                destState.boundingBoxes.emplace_back(destBB);
            }
        }

        for (auto& mine : destState.boundingBoxes)
        {
            if (hasCollision) break;

            for (auto& others : tf->GetBoundingBoxes())
            {
                if (hasCollision) break;

                hasCollision = Collision::HasCollision(mine, others);
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
    TAG_ANIM_CHARACTER currentAnim = m_lowerAnimState;

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
                m_lowerAnimState, true, 0.3f, 0.0f, 0.0f);
       
            //�ϴ� ������ ���� ��
            //set current with same position with finish event m_lowerAnimState
            //D3DXTRACK_DESC lowerDesc;
            //pAnimation->GetLowerTrackDescription(0, &lowerDesc);
            //const float period = pAnimation->GetLowerTrackPeriod(0);
            //float tempNum = period - lowerDesc.Position;
            //cout << tempNum << endl;
            //pAnimation->Set(
            //    CharacterAnimation::BodyPart::BOTH,
            //    currentAnim,
            //    true,
            //    0.5f,
            //    CharacterAnimation::DEFAULT_NEXT_WEIGHT,
            //    lowerDesc.Position,
            //    period - lowerDesc.Position,
            //    [this]()
            //{
            //    pAnimation->Set(
            //        CharacterAnimation::BodyPart::BOTH,
            //        m_lowerAnimState,
            //        true,
            //        0.5f,
            //        CharacterAnimation::DEFAULT_NEXT_WEIGHT);
            //});

            m_savedInput = m_currentStayKey;
        }
    }

    // ī�޶� �������� ������Ʈ (�ֳ��ϸ� ĳ����0 ������Ʈ, ����, ĳ����1 ������Ʈ, ����, ... ������� ����Ǳ� ������)
    CurrentCamera()()->UpdateFrustumCulling();
    
    D3DXVECTOR3 pos = tm->GetPosition();
    D3DXQUATERNION rot = tm->GetRotation();

    headNArmRotation(&m_mouseInput); //���콺 ���Ʒ� -> �ɸ��� �Ӹ��� �� �� �Ʒ��� ������
    cameraCharacterRotation(dt, &rot, &m_mouseInput);//�ɸ��Ϳ� ī�޶��� rotation�� ����ؼ� �ְ� �ȴ�.
    applyTarget_Y_Position(&pos); //apply height and control jumping
    
    
    tm->SetPosition(pos);
    tm->SetRotation(rot);

    // shoot!
    m_isFire = false;
    m_totalInventory.m_bulletFireCoolDown -= dt;
    if (m_totalInventory.m_bulletFireCoolDown <= 0.f) m_totalInventory.m_bulletFireCoolDown = 0.f;
    if (m_attacking == Attacking::Rifle && m_currentOnceKey._LButton)
    {
        if (m_totalInventory.m_bulletFireCoolDown <= 0.f &&  m_totalInventory.m_pHand->GetNumBullet() > 0)
        {
            m_isFire = true;
            //rifleShooting();
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