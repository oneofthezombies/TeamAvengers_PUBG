#include "stdafx.h"
#include "Character.h"
#include "MeshFilterAndAnimator.h"
#include "Renderer.h"
#include "CharacterPart.h"

Character::Character(const int index)
    : IObject()
    , pMeshFilter(nullptr)
    , pAnimator(nullptr)
    , pSkinnedMeshRenderer(nullptr)
    , pWaist(nullptr)
    , m_RotationX(0.0f)
    , pCollisionListener(nullptr)
    , pRoot(nullptr)
    , m_index(index)
{
    pMeshFilter = AddComponent<MeshFilter>();
    pMeshFilter->SetSkinnedMesh("./Resource/Hoon_Sample_Anim/", 
        "Hoon_Sample_Anim.X");

    pAnimator = AddComponent<Animator>();
    pAnimator->SetAnimationIndex(1, false);

    pSkinnedMeshRenderer = AddComponent<SkinnedMeshRenderer>();

    //pCollisionListener = AddComponent<CharacterCollisionListener>();

    //AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::HEAD,
    //    pMeshFilter, pCollisionListener));

    pWaist = static_cast<Frame*>(
        D3DXFrameFind(pMeshFilter->GetRootFrame(), "spine_01"));

    pRoot = static_cast<Frame*>(
        D3DXFrameFind(pMeshFilter->GetRootFrame(), "root"));
}

Character::~Character()
{
}

void Character::OnUpdate()
{
    auto com = g_pCommunicator;
    auto tr = GetTransform();
    auto pos = tr->GetPosition();

    bool isUpdated = false;
    if (m_index == com->m_MyInfo.m_ID)
    {
        //if (g_pInput->IsOnceKeyDown('0'))
        //{
        //    pAnimator->SetAnimationIndex(0, true);
        //}
        //if (g_pInput->IsOnceKeyDown('1'))
        //{
        //    pAnimator->SetAnimationIndex(1, true);
        //}
        //if (g_pInput->IsOnceKeyDown('2'))
        //{
        //    pAnimator->SetAnimationIndex(2, true);
        //}
        //if (g_pInput->IsOnceKeyDown('3'))
        //{
        //    pAnimator->SetAnimationIndex(3, true);
        //}

        //if (g_pInput->IsStayKeyDown('4'))
        //{
        //    m_RotationX += 0.1f;
        //    if (m_RotationX > D3DX_PI * 2.0f)
        //        m_RotationX -= D3DX_PI * 2.0f;
        //}
        //if (g_pInput->IsStayKeyDown('5'))
        //{
        //    m_RotationX -= 0.1f;
        //    if (m_RotationX < D3DX_PI * 2.0f)
        //        m_RotationX += D3DX_PI * 2.0f;
        //}

        if (g_pInput->IsStayKeyDown('6'))
        {
            pos.x += 1.0f;
            isUpdated = true;
        }
        if (g_pInput->IsStayKeyDown('7'))
        {
            pos.x -= 1.0f;
            isUpdated = true;
        }
    }
    else
    {
        auto& playerInfo = com->m_RoomInfo.m_PlayerInfos[m_index];
        pos = playerInfo.m_Position;

        auto pAC = pMeshFilter->GetAnimationController();
        pAC->ResetTime();
        pAC->AdvanceTime(playerInfo.m_AnimationTime, nullptr);
    }

    tr->SetPosition(pos);

    if (isUpdated)
        com->SendPosition(pos);

    pAnimator->Update();

    D3DXMATRIX m;
    D3DXMatrixRotationX(&m, m_RotationX);
    pWaist->TransformationMatrix *= m;

    pRoot->TransformationMatrix *= tr->GetTM();

    pMeshFilter->Update();
}

void Character::OnRender()
{
    pSkinnedMeshRenderer->Render();
}

CharacterCollisionListener::CharacterCollisionListener(IObject* pOwner)
    : ICollisionListener(pOwner)
{
}

CharacterCollisionListener::~CharacterCollisionListener()
{
}

void CharacterCollisionListener::OnCollisionEnter(const Collider& other)
{
    cout << "Enter\n";
}

void CharacterCollisionListener::OnCollisionExit(const Collider& other)
{
    cout << "Exit\n";
}

void CharacterCollisionListener::OnCollisionStay(const Collider& other)
{
    Debug << "Stay\n";
}
