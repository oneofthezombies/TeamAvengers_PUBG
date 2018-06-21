#include "stdafx.h"
#include "Character.h"
#include "MeshFilterAndAnimator.h"
#include "Renderer.h"
#include "CharacterPart.h"

Character::Character()
    : IObject()
    , pMeshFilter(nullptr)
    , pAnimator(nullptr)
    , pSkinnedMeshRenderer(nullptr)
    , pWaist(nullptr)
    , m_RotationX(0.0f)
    , pCollisionListener(nullptr)
    , pRoot(nullptr)
    , m_TranslationX(0.0f)
{
    pMeshFilter = AddComponent<MeshFilter>();
    pMeshFilter->SetSkinnedMesh("./Resource/Lobby_Anim/", "Lobby_Anim.X");

    pAnimator = AddComponent<Animator>();
    pAnimator->SetAnimationIndex(3, false);

    pSkinnedMeshRenderer = AddComponent<SkinnedMeshRenderer>();

    pCollisionListener = AddComponent<CharacterCollisionListener>();

    AddChildren(new CharacterPart(TAG_COLLIDER_CHARACTER_PART::HEAD,
        pMeshFilter, pCollisionListener));

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
    if (g_pInput->IsOnceKeyDown('0'))
    {
        pAnimator->SetAnimationIndex(0, true);
    }
    if (g_pInput->IsOnceKeyDown('1'))
    {
        pAnimator->SetAnimationIndex(1, true);
    }
    if (g_pInput->IsOnceKeyDown('2'))
    {
        pAnimator->SetAnimationIndex(2, true);
    }
    if (g_pInput->IsOnceKeyDown('3'))
    {
        pAnimator->SetAnimationIndex(3, true);
    }
    pAnimator->Update();

    if (g_pInput->IsStayKeyDown('4'))
    {
        m_RotationX += 0.1f;
        if (m_RotationX > D3DX_PI * 2.0f)
            m_RotationX -= D3DX_PI * 2.0f;
    }
    if (g_pInput->IsStayKeyDown('5'))
    {
        m_RotationX -= 0.1f;
        if (m_RotationX < D3DX_PI * 2.0f)
            m_RotationX += D3DX_PI * 2.0f;
    }
    D3DXMATRIX m;
    D3DXMatrixRotationX(&m, m_RotationX);
    pWaist->TransformationMatrix *= m;

    if (g_pInput->IsStayKeyDown('6'))
    {
        m_TranslationX += 0.1f;
    }
    if (g_pInput->IsStayKeyDown('7'))
    {
        m_TranslationX -= 0.1f;
    }
    D3DXMatrixTranslation(&m, m_TranslationX, 0.0f, 0.0f);
    pRoot->TransformationMatrix *= m;

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
