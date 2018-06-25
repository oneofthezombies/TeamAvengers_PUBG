#include "stdafx.h"
#include "Character.h"
#include "CharacterPart.h"
#include "Bullet.h"
#include "SkinnedMeshController.h"

Character::Character(const int index)
    : IObject()

    , m_index(index)
    , m_animState(TAG_ANIM_CHARACTER::Melee_Combat_Stand_Idle_Still)
    , m_rootTransform(1.0f)
    , m_waistRotation(D3DX_PI * 0.5f, 0.1f)
    , m_framePtr()
    , m_pSphereMesh(nullptr)

    , pSkinnedMeshController(nullptr)
{
    pSkinnedMeshController = AddComponent<SkinnedMeshController>();
    pSkinnedMeshController->LoadSkinnedMesh(
        "./Resource/Lobby_Anim/", "Lobby_Anim.x");

    setFramePtr();

    AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::HEAD, this));

    // set sphere mesh for root position
    D3DXCreateSphere(Device()(), 1.0f, 10, 10, &m_pSphereMesh, NULL);

    subscribeCollisionEvent();
}

Character::~Character()
{
    SAFE_RELEASE(m_pSphereMesh);
}

void Character::OnUpdate()
{
    auto pInput = Input()();
    auto pCom = Communication()();
    auto tr = GetTransform();
    auto pos = tr->GetPosition();
    auto isUpdated = false;
    auto isFired = false;
    auto pSkiCon = pSkinnedMeshController;

    if (isMine())
    {
        auto animIndex = pSkiCon->GetCurrentAnimationIndex();
        const auto numAnim = pSkiCon->GetNumAnimation();

        if (pInput->IsOnceKeyDown('1'))
        {
            if (animIndex > 0)
                animIndex--;

            pSkiCon->SetAnimationIndex(animIndex, true);
        }
        if (pInput->IsOnceKeyDown('2'))
        {
            animIndex++;
            if (animIndex >= numAnim)
                animIndex = numAnim - 1;

            pSkiCon->SetAnimationIndex(animIndex, true);
        }

        Debug << "num Anim : " << numAnim << "Anim index : " << animIndex << '\n';

        //if (pInput->IsStayKeyDown('3'))
        //{
        //    RotateWaist(-m_WaistRotation.QUANTITY_FACTOR);
        //}
        //if (pInput->IsStayKeyDown('4'))
        //{
        //    RotateWaist(m_WaistRotation.QUANTITY_FACTOR);
        //}

        //bool isTransit = false;
        //auto nextState = m_AnimState;

        //switch (m_AnimState)
        //{
        //case TAG_ANIM_CHARACTER::Melee_Combat_Stand_Idle_Still:
        //    {
        //        if (pInput->IsOnceKeyDown('W'))
        //        {
        //            nextState = TAG_ANIM_CHARACTER::Melee_Combat_Stand_Walk_F;
        //            isTransit = true;
        //        }
        //    }
        //    break;
        //case TAG_ANIM_CHARACTER::Melee_Combat_Stand_Walk_F:
        //    {
        //        pos += m_RootTransform.m_Direction * 
        //            m_RootTransform.MOVE_SPEED;
        //        isUpdated = true;
        //        
        //        if (!pInput->IsStayKeyDown('W'))
        //        {
        //            nextState = 
        //                TAG_ANIM_CHARACTER::Melee_Combat_Stand_Idle_Still;
        //            isTransit = true;
        //        }
        //    }
        //    break;
        //}
        //if (isTransit)
        //{
        //    m_AnimState = nextState;
        //    const auto ai = static_cast<unsigned int>(m_AnimState);
        //    m_ComponentPtr.pAnimator->SetAnimationIndex(ai, true);
        //    pCom->SendAnimationIndex(ai);
        //}

        if (pInput->IsStayKeyDown('A'))
        {
            D3DXVECTOR3 right;
            D3DXMATRIX r;
            D3DXMatrixRotationY(&r, D3DX_PI * 0.5f);
            D3DXVec3TransformNormal(&right, &m_rootTransform.m_Direction, &r);
            D3DXVec3Normalize(&right, &right);
            pos += right * -m_rootTransform.MOVE_SPEED;
            isUpdated = true;
        }
        if (pInput->IsStayKeyDown('D'))
        {
            D3DXVECTOR3 right;
            D3DXMATRIX r;
            D3DXMatrixRotationY(&r, D3DX_PI * 0.5f);
            D3DXVec3TransformNormal(&right, &m_rootTransform.m_Direction, &r);
            D3DXVec3Normalize(&right, &right);
            pos += right * m_rootTransform.MOVE_SPEED;
            isUpdated = true;
        }

        //if (pInput->IsOnceKeyDown(VK_RETURN))
        //    isFired = true;
    }
    else
    {
        auto& pi = pCom->m_RoomInfo.m_PlayerInfos[m_index];
        pos = pi.m_Position;

        const auto uAnimState = static_cast<unsigned int>(m_animState);
        if (uAnimState != pi.m_AnimationIndex)
        {
            m_animState = static_cast<TAG_ANIM_CHARACTER>(pi.m_AnimationIndex);
            pSkiCon->SetAnimationIndex(pi.m_AnimationIndex, true);
        }
    }

    tr->SetPosition(pos);

    updateTransform();

    if (isMine())
    {
        if (isFired)
        {
            D3DXQUATERNION rot;
            D3DXQuaternionRotationAxis(&rot, &Vector3::UP, D3DX_PI * 0.5f);
            auto bullet = BulletPool()()->Fire(Matrix::GetTranslation(
                m_framePtr.pHandGun->CombinedTransformationMatrix 
              * tr->GetTransformationMatrix()),
                rot, 0.1f, 10.0f, GetTagCollisionDamage(m_index));
            pCom->SendEventFireBullet(bullet);
        }

        if (isUpdated)
            pCom->SendPosition(pos);
    }
}

void Character::OnRender()
{
    pSkinnedMeshController->Render();

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
