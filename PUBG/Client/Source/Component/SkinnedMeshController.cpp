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
        pMeshContainer->m_pFinalBoneMatrices[i] 
            = pMeshContainer->m_pBoneOffsetMatrices[i] 
            * *pMeshContainer->m_ppBoneMatrixPtrs[i];
    }

    PBYTE pVerticesSrc = nullptr;
    pMeshContainer->pEffectMesh->m_pMesh->LockVertexBuffer(
        D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);

    assert(pVerticesSrc && 
        "SkinnedMeshController::drawMeshContainer(), \
         source vertices is null.");

    PBYTE pVerticesDest = nullptr;
    pMeshContainer->m_pWorkMesh->LockVertexBuffer(0, (LPVOID*)&pVerticesDest);

    assert(pVerticesDest &&         
        "SkinnedMeshController::drawMeshContainer(), \
         destination vertices is null.");

    pMeshContainer->pSkinInfo->UpdateSkinnedMesh(
        pMeshContainer->m_pFinalBoneMatrices, nullptr, 
        pVerticesSrc, pVerticesDest);

    pMeshContainer->pEffectMesh->m_pMesh->UnlockVertexBuffer();
    pMeshContainer->m_pWorkMesh->UnlockVertexBuffer();

    pMeshContainer->pEffectMesh->Render(
        GetTransform()->GetTransformationMatrix(), 
        pMeshContainer->m_pWorkMesh);
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
    , m_currentIndex(0)
    , m_totalBlendTime(0.3f)
    , m_passedBlendTime(0.0f)

    , pSkinnedMesh(nullptr)
{
}

SkinnedMeshController::~SkinnedMeshController()
{
}

void SkinnedMeshController::Update(const function<void()>& function)
{
    assert(pSkinnedMesh && 
        "SkinnedMeshController::Update(), skinned mesh is null.");

    const auto dt = Time()()->GetDeltaTime();
    auto& pAniCon = pSkinnedMesh->m_pAnimController;
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

    updateFrameToWorld(pSkinnedMesh->m_pRootFrame, nullptr);
}

void SkinnedMeshController::Render()
{
    assert(pSkinnedMesh && 
        "SkinnedMeshController::Render(), skinned mesh is null.");

    drawFrame(pSkinnedMesh->m_pRootFrame);
}

void SkinnedMeshController::SetSkinnedMesh(SkinnedMesh* pSkinnedMesh)
{
    assert(pSkinnedMesh && 
        "SkinnedMeshController::SetSkinnedMesh(), skinned mesh is null.");

    this->pSkinnedMesh = pSkinnedMesh;
    this->pSkinnedMesh->Setup();
}

void SkinnedMeshController::LoadSkinnedMesh(
    const string& path, const string& xFilename)
{
    pSkinnedMesh = loadSkinnedMesh(path, xFilename);
    pSkinnedMesh->Setup();
}

void SkinnedMeshController::LoadAdditionalAnimation(
    const string& path, const string& xFilename)
{
    SkinnedMesh* pAddSkinnedMesh = loadSkinnedMesh(path, xFilename);

    LPD3DXANIMATIONCONTROLLER pNew = nullptr;
    auto& pOld = pSkinnedMesh->m_pAnimController;
    auto& pAdd = pAddSkinnedMesh->m_pAnimController;

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

    SAFE_DELETE(pAddSkinnedMesh);
}

void SkinnedMeshController::SetAnimationIndex(
    const size_t index, const bool isBlend, 
    const float currentWeight, const float nextWeight, const float blendTime)
{
    LPD3DXANIMATIONSET pNext = nullptr;
    auto& pAniCon = pSkinnedMesh->m_pAnimController;

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
    assert(pSkinnedMesh && pSkinnedMesh->m_pAnimController && 
        "SkinnedMeshController::GetNumAnimation(), pointer is null.");
    
    return pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets();
}

Frame* SkinnedMeshController::FindFrame(const string& name)
{
    assert(pSkinnedMesh &&
        "SkinnedMeshController::FindFrame(), skinned mesh is null.");

    auto pFrame = D3DXFrameFind(pSkinnedMesh->m_pRootFrame, name.c_str());

    assert(pFrame && 
        "SkinnedMeshController::FindFrame(), D3DXFrameFind() failed.");

    return static_cast<Frame*>(pFrame);
}
