#include "stdafx.h"
#include "SkinnedMeshController.h"

void SkinnedMeshController::updateFrameToWorld(LPD3DXFRAME pFrameBase, 
    LPD3DXFRAME pParent)
{
    if (!pFrameBase) return;

    auto pFrame = static_cast<Frame*>(pFrameBase);
    pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

    if (pParent)
    {
        pFrame->CombinedTransformationMatrix *= 
            static_cast<Frame*>(pParent)->CombinedTransformationMatrix;
    }

    updateFrameToWorld(pFrame->pFrameSibling, pParent);
    updateFrameToWorld(pFrame->pFrameFirstChild, pFrame);
}

void SkinnedMeshController::drawFrame(LPD3DXFRAME pFrameBase)
{
    if (!pFrameBase) return;

    auto pMeshContainer = pFrameBase->pMeshContainer;
    while (pMeshContainer)
    {
        drawMeshContainer(pMeshContainer);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    drawFrame(pFrameBase->pFrameSibling);
    drawFrame(pFrameBase->pFrameFirstChild);
}

void SkinnedMeshController::drawMeshContainer(
    LPD3DXMESHCONTAINER pMeshContainerBase)
{
    if (!pMeshContainerBase || !pMeshContainerBase->pSkinInfo) return;

    auto pMeshContainer = static_cast<MeshContainer*>(pMeshContainerBase);
    auto numBones = pMeshContainer->pSkinInfo->GetNumBones();
    for (auto i = 0u; i < numBones; ++i)
    {
        pMeshContainer->pFinalBoneMatrices[i] 
            = pMeshContainer->pBoneOffsetMatrices[i] 
            * *pMeshContainer->ppBoneMatrixPtrs[i];
    }

    PBYTE pVerticesSrc = nullptr;
    pMeshContainer->pEffectMesh->pMesh->LockVertexBuffer(
        D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);

    assert(pVerticesSrc && 
        "SkinnedMeshController::drawMeshContainer(), \
         source vertices is null.");

    PBYTE pVerticesDest = nullptr;
    pMeshContainer->pWorkMesh->LockVertexBuffer(0, (LPVOID*)&pVerticesDest);

    assert(pVerticesDest &&         
        "SkinnedMeshController::drawMeshContainer(), \
         destination vertices is null.");

    pMeshContainer->pSkinInfo->UpdateSkinnedMesh(
        pMeshContainer->pFinalBoneMatrices, nullptr, 
        pVerticesSrc, pVerticesDest);

    pMeshContainer->pEffectMesh->pMesh->UnlockVertexBuffer();
    pMeshContainer->pWorkMesh->UnlockVertexBuffer();

    pMeshContainer->pEffectMesh->Render(
        GetTransform()->GetTransformationMatrix(), pMeshContainer->pWorkMesh);
}

SkinnedMesh* SkinnedMeshController::loadSkinnedMesh(
    const string& path, const string& xFilename)
{
    SkinnedMesh* pSkinnedMesh = new SkinnedMesh;
    AllocateHierarchy allocator(path, xFilename);

    const auto hr = D3DXLoadMeshHierarchyFromXA(
        (path + xFilename).c_str(), D3DXMESH_MANAGED, Device()(), &allocator, 
        nullptr, &pSkinnedMesh->m_pRootFrame, 
        &pSkinnedMesh->m_pAnimController);

    assert(!FAILED(hr) &&
        "SkinnedMeshController::loadSkinnedMesh(), \
         D3DXLoadMeshHierarchyFromXA() failed.");

    return pSkinnedMesh;
}

SkinnedMeshController::SkinnedMeshController(IObject* pOwner)
    : Component(pOwner)
    , m_pSkinnedMesh(nullptr)
    , m_currentIndex(0)
    , m_totalBlendTime(0.3f)
    , m_passedBlendTime(0.0f)
{
}

SkinnedMeshController::~SkinnedMeshController()
{
    SAFE_DELETE(m_pSkinnedMesh);
}

void SkinnedMeshController::Update(const function<void()>& function)
{
    assert(m_pSkinnedMesh && 
        "SkinnedMeshController::Update(), skinned mesh is null.");

    const auto dt = Time()()->GetDeltaTime();
    auto& pAniCon = m_pSkinnedMesh->m_pAnimController;
    pAniCon->AdvanceTime(dt, nullptr);

    if (m_passedBlendTime < m_totalBlendTime)
    {
        m_passedBlendTime += dt;
        const auto weight = m_passedBlendTime / m_totalBlendTime;
        if (weight >= 1.0f)
        {
            pAniCon->SetTrackWeight(0u, 1.0f);
            pAniCon->SetTrackWeight(1u, 0.0f);
            pAniCon->SetTrackEnable(1u, false);
        }
        else
        {
            pAniCon->SetTrackWeight(0u, weight);
            pAniCon->SetTrackWeight(1u, 1.0f - weight);
        }
    }

    if (function) function();

    updateFrameToWorld(m_pSkinnedMesh->m_pRootFrame, nullptr);
}

void SkinnedMeshController::Render()
{
    assert(m_pSkinnedMesh && 
        "SkinnedMeshController::Render(), skinned mesh is null.");

    drawFrame(m_pSkinnedMesh->m_pRootFrame);
}

void SkinnedMeshController::SetSkinnedMesh(SkinnedMesh* pSkinnedMesh)
{
    assert(pSkinnedMesh && 
        "SkinnedMeshController::SetSkinnedMesh(), skinned mesh is null.");

    m_pSkinnedMesh = pSkinnedMesh;
}

void SkinnedMeshController::LoadSkinnedMesh(
    const string& path, const string& xFilename)
{
    m_pSkinnedMesh = loadSkinnedMesh(path, xFilename);
    m_pSkinnedMesh->Setup();
}

void SkinnedMeshController::LoadAdditionalAnimation(
    const string& path, const string& xFilename)
{
    SkinnedMesh* pSkinnedMesh = loadSkinnedMesh(path, xFilename);

    LPD3DXANIMATIONCONTROLLER pNew = nullptr;
    auto& pOld = m_pSkinnedMesh->m_pAnimController;
    auto& pAdd = pSkinnedMesh->m_pAnimController;

    pOld->CloneAnimationController(
        pOld->GetMaxNumAnimationOutputs(),
        pOld->GetMaxNumAnimationSets() + pAdd->GetMaxNumAnimationSets(), 
        pOld->GetMaxNumTracks(),
        pOld->GetMaxNumEvents(),
        &pNew);

    LPD3DXANIMATIONSET pAddSet = nullptr;
    auto numAddSet = pAdd->GetNumAnimationSets();
    for (auto i = 0u; i < numAddSet; ++i)
    {
        pAdd->GetAnimationSet(i, &pAddSet);
        pNew->RegisterAnimationSet(pAddSet);
        pAddSet->Release();
    }

    pOld->Release();
    pOld = pNew;

    SAFE_DELETE(pSkinnedMesh);
}

void SkinnedMeshController::SetAnimationIndex(
    const size_t index, const bool isBlend, 
    const float currentWeight, const float nextWeight, const float blendTime)
{
    LPD3DXANIMATIONSET pNext = nullptr;
    auto& pAniCon = m_pSkinnedMesh->m_pAnimController;

    auto hr = pAniCon->GetAnimationSet(index, &pNext);
    assert(!FAILED(hr) && 
        "SkinnedMeshController::SetAnimationIndex(), \
         ID3DXAnimationController::GetAnimationSet() failed.");

    m_currentIndex = index;

    if (isBlend)
    {
        LPD3DXANIMATIONSET pCurrent = nullptr;
        pAniCon->GetTrackAnimationSet(0u, &pCurrent);
        pAniCon->SetTrackAnimationSet(1u, pCurrent);

        D3DXTRACK_DESC desc;
        pAniCon->GetTrackDesc(0u, &desc);
        pAniCon->SetTrackDesc(1u, &desc);

        pAniCon->SetTrackWeight(0u, nextWeight);
        pAniCon->SetTrackWeight(1u, currentWeight);

        SAFE_RELEASE(pCurrent);

        m_passedBlendTime = 0.0f;
        m_totalBlendTime = blendTime;
    }

    pAniCon->SetTrackAnimationSet(0u, pNext);
    //pAniCon->ResetTime();
    pAniCon->SetTrackPosition(0u, 0.0);

    SAFE_RELEASE(pNext);
}

size_t SkinnedMeshController::GetCurrentAnimationIndex() const
{
    return m_currentIndex;
}

size_t SkinnedMeshController::GetNumAnimation() const
{
    assert(m_pSkinnedMesh && m_pSkinnedMesh->m_pAnimController && 
        "SkinnedMeshController::GetNumAnimation(), pointer is null.");
    
    return m_pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets();
}

Frame* SkinnedMeshController::FindFrame(const string& name)
{
    assert(m_pSkinnedMesh &&
        "SkinnedMeshController::FindFrame(), skinned mesh is null.");

    auto pFrame = D3DXFrameFind(m_pSkinnedMesh->m_pRootFrame, name.c_str());

    assert(pFrame && 
        "SkinnedMeshController::FindFrame(), D3DXFrameFind() failed.");

    return static_cast<Frame*>(pFrame);
}
