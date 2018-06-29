#include "stdafx.h"
#include "SkinnedMeshController.h"
#include "TagAnimation.h"

void SkinnedMeshController::updateFrameToModelSpace(LPD3DXFRAME pFrameBase,
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

    updateFrameToModelSpace(pFrame->pFrameSibling, pParent);
    updateFrameToModelSpace(pFrame->pFrameFirstChild, pFrame);
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

    //pMeshContainer->pEffectMesh->Render(
    //    GetTransform()->GetTransformationMatrix(), 
    //    pMeshContainer->m_pWorkMesh);

    pMeshContainer->pEffectMesh->Render(
        pMeshContainer->m_pWorkMesh, 
        [this](LPD3DXEFFECT pEffect) 
    {
        const GlobalVariableKey* pkey = Shader()()->GetGlobalVariableKey();
        pEffect->SetMatrix(
            pkey->m_pWorld, 
            &GetTransform()->GetTransformationMatrix());

    });
}

SkinnedMeshController::SkinnedMeshController(IObject* pOwner)
    : Component(pOwner)
    , m_currentAnimIndex(0)
    , m_currentAnimName("")
    , m_totalBlendTime(0.3f)
    , m_passedBlendTime(0.0f)
    , m_pSkinnedMeshInstance(nullptr)
{
}

SkinnedMeshController::~SkinnedMeshController()
{
    SAFE_DELETE(m_pSkinnedMeshInstance);
}

void SkinnedMeshController::UpdateAnimation()
{
    assert(m_pSkinnedMeshInstance &&
        "SkinnedMeshController::UpdateAnimation(), \
         skinned mesh instance is null.");

    const auto dt = Time()()->GetDeltaTime();
    auto& pAniCon = m_pSkinnedMeshInstance->m_pAnimController;
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
}

void SkinnedMeshController::UpdateModel()
{
    updateFrameToModelSpace(
        m_pSkinnedMeshInstance->pSkinnedMesh->m_pRootFrame, nullptr);
}

void SkinnedMeshController::Render()
{
    assert(m_pSkinnedMeshInstance &&
        "SkinnedMeshController::Render(), skinned mesh is null.");

    drawFrame(m_pSkinnedMeshInstance->pSkinnedMesh->m_pRootFrame);
}

void SkinnedMeshController::SetSkinnedMesh(SkinnedMesh* pSkinnedMesh)
{
    assert(pSkinnedMesh && 
        "SkinnedMeshController::SetSkinnedMesh(), skinned mesh is null.");

    SAFE_DELETE(m_pSkinnedMeshInstance);
    m_pSkinnedMeshInstance = new SkinnedMeshInstance;

    m_pSkinnedMeshInstance->pSkinnedMesh = pSkinnedMesh;

    LPD3DXANIMATIONCONTROLLER pSrc = pSkinnedMesh->m_pAnimController;
    LPD3DXANIMATIONCONTROLLER pDest = nullptr;

    pSrc->CloneAnimationController(
        pSrc->GetMaxNumAnimationOutputs(),
        pSrc->GetMaxNumAnimationSets(), 
        pSrc->GetMaxNumTracks(), 
        pSrc->GetMaxNumEvents(), 
        &pDest);

    m_pSkinnedMeshInstance->m_pAnimController = pDest;
}

void SkinnedMeshController::SetAnimationIndex(
    const size_t index, const bool isBlend, 
    const float currentWeight, const float nextWeight, const float blendTime)
{
    LPD3DXANIMATIONSET pNext = nullptr;
    auto& pAniCon = m_pSkinnedMeshInstance->m_pAnimController;

    HRESULT hr = pAniCon->GetAnimationSet(index, &pNext);
    assert(!FAILED(hr) && 
        "SkinnedMeshController::SetAnimationIndex(), \
         ID3DXAnimationController::GetAnimationSet() failed.");

    m_currentAnimIndex = index;

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

void SkinnedMeshController::SetAnimation(
    const string& name, const bool isBlend, const float currentWeight, 
    const float nextWeight, const float blendTime)
{
    LPD3DXANIMATIONSET pNext = nullptr;
    auto& pAniCon = m_pSkinnedMeshInstance->m_pAnimController;

    HRESULT hr = pAniCon->GetAnimationSetByName(name.c_str(), &pNext);

    assert(!FAILED(hr) &&
        "SkinnedMeshController::SetAnimationIndex(), \
         ID3DXAnimationController::GetAnimationSet() failed.");

    m_currentAnimName = name;

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
    return m_currentAnimIndex;
}

const string & SkinnedMeshController::GetCurrentAnimationName() const
{
    return m_currentAnimName;
}

size_t SkinnedMeshController::GetNumAnimation() const
{
    assert(
        m_pSkinnedMeshInstance && 
        m_pSkinnedMeshInstance->m_pAnimController &&
        "SkinnedMeshController::GetNumAnimation(), pointer is null.");
    
    return m_pSkinnedMeshInstance->m_pAnimController->GetMaxNumAnimationSets();
}

Frame* SkinnedMeshController::FindFrame(const string& name)
{
    assert(m_pSkinnedMeshInstance &&
        "SkinnedMeshController::FindFrame(), skinned mesh instance is null.");

    auto pFrame = D3DXFrameFind(
        m_pSkinnedMeshInstance->pSkinnedMesh->m_pRootFrame, name.c_str());

    assert(pFrame && 
        "SkinnedMeshController::FindFrame(), D3DXFrameFind() failed.");

    return static_cast<Frame*>(pFrame);
}
