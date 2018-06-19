#include "stdafx.h"
#include "Animator.h"

void Animator::UpdateAnim(const float dt)
{
    auto& pAC = m_pSkinnedMesh->pAnimController;

    pAC->AdvanceTime(dt, nullptr);

    if (m_PassedBlendTime > m_BlendTime) return;

    m_PassedBlendTime += dt;

    if (m_PassedBlendTime >= m_BlendTime)
    {
        pAC->SetTrackWeight(0u, 1.0f);
        pAC->SetTrackWeight(1u, 0.0f);
        pAC->SetTrackEnable(1u, false);
    }
    else
    {
        const float weight = m_PassedBlendTime / m_BlendTime;
        pAC->SetTrackWeight(0u, weight);
        pAC->SetTrackWeight(1u, 1.0f - weight);
    }
}

void Animator::UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
    if (!pFrame) return;

    auto pF = static_cast<Frame*>(pFrame);
    pF->CombinedTM = pF->TransformationMatrix;
    if (pParent)
        pF->CombinedTM *= static_cast<Frame*>(pParent)->CombinedTM;

    UpdateFrameMatrices(pFrame->pFrameSibling, pParent);
    UpdateFrameMatrices(pFrame->pFrameFirstChild, pFrame);
}

Animator::Animator(IObject* pOwner)
    : Component(pOwner)
    , m_pSkinnedMesh(nullptr)
    , m_Index(0)
    , m_BlendTime(0.3f)
    , m_PassedBlendTime(0.0f)
{
}

Animator::~Animator()
{
}

void Animator::Update()
{
    UpdateAnim(g_pTime->GetDeltaTime());
    UpdateFrameMatrices(m_pSkinnedMesh->pRootFrame, nullptr);
}

void Animator::SetAnimationIndex(const unsigned int index, const bool isBlend)
{
    auto& pAC = m_pSkinnedMesh->pAnimController;

    LPD3DXANIMATIONSET pNext = nullptr;
    auto hr = pAC->GetAnimationSet(index, &pNext);
    assert(!FAILED(hr) && "Animator::SetAnimationIndex() failed.");

    if (isBlend)
    {
        LPD3DXANIMATIONSET pPrev = nullptr;
        pAC->GetTrackAnimationSet(0u, &pPrev);
        pAC->SetTrackAnimationSet(1u, pPrev);

        D3DXTRACK_DESC desc;
        pAC->GetTrackDesc(0u, &desc);
        pAC->SetTrackDesc(1u, &desc);

        pAC->SetTrackWeight(0u, 0.0f);
        pAC->SetTrackWeight(1u, 1.0f);

        SAFE_RELEASE(pPrev);

        m_PassedBlendTime = 0.0f;
    }

    pAC->SetTrackAnimationSet(0u, pNext);
    pAC->ResetTime();

    SAFE_RELEASE(pNext);
}

void Animator::SetSkinnedMesh(SkinnedMesh* p)
{
    if (!p) return;

    m_pSkinnedMesh = p;
}

LPD3DXFRAME Animator::GetRootFrame()
{
    if (!m_pSkinnedMesh) return nullptr;
    return m_pSkinnedMesh->pRootFrame;
}



