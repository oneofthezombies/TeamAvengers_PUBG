#include "stdafx.h"
#include "MeshFilterAndAnimator.h"

void Animator::UpdateAnim(const float dt, LPD3DXANIMATIONCONTROLLER pAnimController)
{
    auto& pAC = pAnimController;

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

Animator::Animator(IObject* pOwner)
    : Component(pOwner)
    , m_pMeshFilter(nullptr)
    , m_Index(0)
    , m_BlendTime(0.3f)
    , m_PassedBlendTime(0.0f)
    , m_TotalTime(0.0f)
    , m_PassedTime(0.0f)
{
}

Animator::~Animator()
{
}

void Animator::Update()
{
    if (!m_pMeshFilter)
        m_pMeshFilter = GetComponent<MeshFilter>();

    assert(m_pMeshFilter && "Animator::Update() failed. MeshFilter is null.");

    UpdateAnim(g_pTime->GetDeltaTime(), 
        m_pMeshFilter->GetAnimationController());
}

void Animator::SetAnimationIndex(const unsigned int index, const bool isBlend)
{
    if (!m_pMeshFilter)
        m_pMeshFilter = GetComponent<MeshFilter>();

    if (!m_pMeshFilter) return;

    auto pAC = m_pMeshFilter->GetAnimationController();

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

MeshFilter::MeshFilter(IObject* pOwner)
    : Component(pOwner)
    , m_pSkinnedMesh(nullptr)
{
}

MeshFilter::~MeshFilter()
{
    if (m_pSkinnedMesh)
        g_pResourceManager->RemoveSkinnedMesh(m_pSkinnedMesh);
}

void MeshFilter::Update()
{
    if (m_pSkinnedMesh)
        UpdateFrameMatrices(m_pSkinnedMesh->pRootFrame, nullptr);
}

void MeshFilter::SetSkinnedMesh(SkinnedMesh* p)
{
    if (!p) return;

    m_pSkinnedMesh = p;
}

void MeshFilter::SetSkinnedMesh(const string& path, const string& xFilename)
{
    SetSkinnedMesh(g_pResourceManager->GetSkinnedMesh(path, xFilename));
}

LPD3DXFRAME MeshFilter::GetRootFrame()
{
    if (!m_pSkinnedMesh) return nullptr;
    return m_pSkinnedMesh->pRootFrame;
}

LPD3DXANIMATIONCONTROLLER MeshFilter::GetAnimationController()
{
    if (!m_pSkinnedMesh) return nullptr;
    return m_pSkinnedMesh->pAnimController;
}

Frame* MeshFilter::FindFrame(const string& name)
{
    if (!m_pSkinnedMesh) return nullptr;
    return static_cast<Frame*>(D3DXFrameFind(m_pSkinnedMesh->pRootFrame, 
        name.c_str()));
}

void MeshFilter::UpdateFrameMatrices(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
    if (!pFrame) return;

    auto pF = static_cast<Frame*>(pFrame);
    pF->CombinedTM = pF->TransformationMatrix;
    if (pParent)
        pF->CombinedTM *= static_cast<Frame*>(pParent)->CombinedTM;

    UpdateFrameMatrices(pFrame->pFrameSibling, pParent);
    UpdateFrameMatrices(pFrame->pFrameFirstChild, pFrame);
}