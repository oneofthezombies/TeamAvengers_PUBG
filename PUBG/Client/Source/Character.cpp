#include "stdafx.h"
#include "Character.h"
#include "MeshFilterAndAnimator.h"
#include "Renderer.h"
#include "CharacterPart.h"
#include "CharacterCollisionListener.h"

Character::Character(const int index)
    : IObject()

    , m_Index(index)
    , m_RootTransform(1.0f)
    , m_WaistRotation(D3DX_PI * 0.5f, 0.1f)
    , m_pSphereMesh(nullptr)
    , m_AnimState(TAG_ANIM_CHARACTER::Melee_Combat_Stand_Idle_Still)

    , pMeshFilter(nullptr)
    , pAnimator(nullptr)
    , pSkinnedMeshRenderer(nullptr)
    , pCollisionListener(nullptr)

    , pWaist(nullptr)
    , pRoot(nullptr)
{
    pMeshFilter = AddComponent<MeshFilter>();
    pMeshFilter->SetSkinnedMesh(
        "./Resource/Test_Anim/", "Test_Anim.x");

    pAnimator = AddComponent<Animator>();
    pAnimator->SetAnimationIndex(
        static_cast<int>(
            TAG_ANIM_CHARACTER::Melee_Combat_Stand_Idle_Still), false);

    pSkinnedMeshRenderer = AddComponent<SkinnedMeshRenderer>();

    pCollisionListener = AddComponent<CharacterCollisionListener>();

    AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::HEAD, this));

    pWaist = static_cast<Frame*>(
        D3DXFrameFind(pMeshFilter->GetRootFrame(), "spine_01"));

    pRoot = static_cast<Frame*>(
        D3DXFrameFind(pMeshFilter->GetRootFrame(), "root"));

    D3DXCreateSphere(g_pDevice, 1.0f, 10, 10, &m_pSphereMesh, NULL);
}

Character::~Character()
{
    SAFE_RELEASE(m_pSphereMesh);
}

Character::WaistRotation::WaistRotation(const float limit, const float factor)
    : LIMIT_OF_ANGLE(limit)
    , QUANTITY_FACTOR(factor)
    , m_Angle(0.0f)
{
}

Character::RootTransform::RootTransform(const float moveSpeed)
    : MOVE_SPEED(moveSpeed)
    , m_Direction(0.0f, 0.0f, -1.0f)
{
}

void Character::OnUpdate()
{
    auto pInput = g_pInput;
    auto pCom = g_pCommunicator;
    auto tr = GetTransform();
    auto pos = tr->GetPosition();
    auto isUpdated = false;

    if (IsMine())
    {
        //auto animIndex = pAnimator->GetCurrentAnimationIndex();

        //if (pInput->IsOnceKeyDown('1'))
        //{
        //    animIndex--;
        //    if (animIndex < 0)
        //        animIndex = 0;

        //    pAnimator->SetAnimationIndex(animIndex, true);
        //}
        //if (pInput->IsOnceKeyDown('2'))
        //{
        //    const auto numAnim = pAnimator->GetNumAnimation();

        //    animIndex++;
        //    if (animIndex >= numAnim)
        //        animIndex = numAnim - 1;

        //    pAnimator->SetAnimationIndex(animIndex, true);
        //}

        //Debug << "Anim index : " << animIndex << '\n';

        //if (pInput->IsStayKeyDown('3'))
        //{
        //    RotateWaist(-m_WaistRotation.QUANTITY_FACTOR);
        //}
        //if (pInput->IsStayKeyDown('4'))
        //{
        //    RotateWaist(m_WaistRotation.QUANTITY_FACTOR);
        //}

        bool isTransit = false;
        auto nextState = m_AnimState;

        switch (m_AnimState)
        {
        case TAG_ANIM_CHARACTER::Melee_Combat_Stand_Idle_Still:
            {
                if (pInput->IsOnceKeyDown('W'))
                {
                    nextState = TAG_ANIM_CHARACTER::Melee_Combat_Stand_Walk_F;
                    isTransit = true;
                }
            }
            break;
        case TAG_ANIM_CHARACTER::Melee_Combat_Stand_Walk_F:
            {
                pos += m_RootTransform.m_Direction * 
                    m_RootTransform.MOVE_SPEED;
                isUpdated = true;
                
                if (!pInput->IsStayKeyDown('W'))
                {
                    nextState = 
                        TAG_ANIM_CHARACTER::Melee_Combat_Stand_Idle_Still;
                    isTransit = true;
                }
            }
            break;
        }
        if (isTransit)
        {
            m_AnimState = nextState;
            const auto ai = static_cast<unsigned int>(m_AnimState);
            pAnimator->SetAnimationIndex(ai, true);
            pCom->SendAnimationIndex(ai);
        }

        if (pInput->IsStayKeyDown('A'))
        {
            D3DXVECTOR3 right;
            D3DXMATRIX r;
            D3DXMatrixRotationY(&r, D3DX_PI * 0.5f);
            D3DXVec3TransformNormal(&right, &m_RootTransform.m_Direction, &r);
            D3DXVec3Normalize(&right, &right);
            pos += right * -m_RootTransform.MOVE_SPEED;
            isUpdated = true;
        }
        if (pInput->IsStayKeyDown('D'))
        {
            D3DXVECTOR3 right;
            D3DXMATRIX r;
            D3DXMatrixRotationY(&r, D3DX_PI * 0.5f);
            D3DXVec3TransformNormal(&right, &m_RootTransform.m_Direction, &r);
            D3DXVec3Normalize(&right, &right);
            pos += right * m_RootTransform.MOVE_SPEED;
            isUpdated = true;
        }
    }
    else
    {
        auto& pi = pCom->m_RoomInfo.m_PlayerInfos[m_Index];
        pos = pi.m_Position;

        const auto uAnimState = static_cast<unsigned int>(m_AnimState);
        if (uAnimState != pi.m_AnimationIndex)
        {
            m_AnimState = static_cast<TAG_ANIM_CHARACTER>(pi.m_AnimationIndex);
            pAnimator->SetAnimationIndex(pi.m_AnimationIndex, true);
        }
    }

    tr->SetPosition(pos);
    Debug << "Position : " << pos << '\n';

    Debug << "Position2 : " << Matrix::GetTranslation(tr->GetTransformationMatrix()) << '\n';

    if (isUpdated)
        pCom->SendPosition(pos);

    UpdateTransform();
}

void Character::OnRender()
{
    pSkinnedMeshRenderer->Render();

    auto pDevice = g_pDevice;
    pDevice->SetRenderState(D3DRS_LIGHTING, true);
     
    pDevice->SetTexture(0, NULL);
    pDevice->SetMaterial(&MaterialTemplate::GetWhite());

    auto world = GetTransform()->GetTransformationMatrix();
    pDevice->SetTransform(D3DTS_WORLD, &world);
    m_pSphereMesh->DrawSubset(0);

    for (int i = 0; i < 10; ++i)
    {
        D3DXMATRIX t;
        D3DXMatrixTranslation(&t, static_cast<float>(i * 10), 0.0f, 0.0f);
        pDevice->SetTransform(D3DTS_WORLD, &t);
        m_pSphereMesh->DrawSubset(0);
    }
}

bool Character::IsMine() const
{
    return m_Index == g_pCommunicator->m_MyInfo.m_ID;
}

void Character::UpdateTransform()
{
    pAnimator->Update();

    D3DXMATRIX m;
    D3DXMatrixRotationX(&m, m_WaistRotation.m_Angle);
    pWaist->TransformationMatrix *= m;

    pRoot->TransformationMatrix = Matrix::IDENTITY;
    auto rootPos = Matrix::GetTranslation(pRoot->TransformationMatrix);
    Debug << "Root Positon : " << rootPos << '\n';

    pMeshFilter->Update();
}

void Character::RotateWaist(const float quantity)
{
    auto& wr = m_WaistRotation;

    wr.m_Angle += quantity;
    
    if (wr.m_Angle < -wr.LIMIT_OF_ANGLE)
        wr.m_Angle = -wr.LIMIT_OF_ANGLE;
    else if (wr.m_Angle > wr.LIMIT_OF_ANGLE)
        wr.m_Angle = wr.LIMIT_OF_ANGLE;
}

