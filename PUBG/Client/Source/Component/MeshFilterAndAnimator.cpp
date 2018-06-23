#include "stdafx.h"
#include "MeshFilterAndAnimator.h"

void Animator::updateAnim()
{
    const auto dt = g_pTime->GetDeltaTime();

    pAnimController->AdvanceTime(dt, nullptr);

    if (m_PassedBlendTime > m_BlendTime) return;

    m_PassedBlendTime += dt;

    if (m_PassedBlendTime >= m_BlendTime)
    {
        pAnimController->SetTrackWeight(0u, 1.0f);
        pAnimController->SetTrackWeight(1u, 0.0f);
        pAnimController->SetTrackEnable(1u, false);
    }
    else
    {
        const float weight = m_PassedBlendTime / m_BlendTime;
        pAnimController->SetTrackWeight(0u, weight);
        pAnimController->SetTrackWeight(1u, 1.0f - weight);
    }
}

void Animator::setMeshFilterPtr()
{
    if (pMeshFilter) return;

    pMeshFilter = GetComponent<MeshFilter>();
    pAnimController = pMeshFilter->GetAnimationController();

    assert((pMeshFilter || pAnimController) &&
        "Animator::GetMeshFilter() failed. MeshFilter is null.");
}

Animator::Animator(IObject* pOwner)
    : Component(pOwner)

    , m_Index(0)
    , m_BlendTime(0.3f)
    , m_PassedBlendTime(0.0f)
    , m_TotalTime(0.0f)
    , m_PassedTime(0.0f)

    , pMeshFilter(nullptr)
    , pAnimController(nullptr)
{
}

Animator::~Animator()
{
}

void Animator::Update()
{
    setMeshFilterPtr();

    updateAnim();
}

void Animator::SetAnimationIndex(const unsigned int index, const bool isBlend)
{
    setMeshFilterPtr();

    LPD3DXANIMATIONSET pNext = nullptr;
    auto hr = pAnimController->GetAnimationSet(index, &pNext);
    assert(!FAILED(hr) && "Animator::SetAnimationIndex() failed.");

    m_Index = index;

    if (isBlend)
    {
        LPD3DXANIMATIONSET pPrev = nullptr;
        pAnimController->GetTrackAnimationSet(0u, &pPrev);
        pAnimController->SetTrackAnimationSet(1u, pPrev);

        D3DXTRACK_DESC desc;
        pAnimController->GetTrackDesc(0u, &desc);
        pAnimController->SetTrackDesc(1u, &desc);

        pAnimController->SetTrackWeight(0u, 0.0f);
        pAnimController->SetTrackWeight(1u, 1.0f);

        SAFE_RELEASE(pPrev);

        m_PassedBlendTime = 0.0f;
    }

    pAnimController->SetTrackAnimationSet(0u, pNext);
    pAnimController->ResetTime();

    SAFE_RELEASE(pNext);
}

int Animator::GetCurrentAnimationIndex()
{
    setMeshFilterPtr();

    return m_Index;
}

int Animator::GetNumAnimation()
{
    setMeshFilterPtr();

    return static_cast<int>(pAnimController->GetMaxNumAnimationSets());
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