#include "stdafx.h"
#include "SkinnedMeshController.h"
#include "TagAnimation.h"

NextAnimation::NextAnimation()
    : name("")
    , nextSpeed(0.0f)
    , isBlend(false)
    , blendTime(0.0f)
    , nextWeight(0.0f)
{
}

void SkinnedMeshController::checkIsFinishedAndSetNextAnimation(const float calcedDt)
{
    auto& pAniCon = m_pSkinnedMeshInstance->m_pAnimController;

    D3DXTRACK_DESC desc;
    pAniCon->GetTrackDesc(0, &desc);
    LPD3DXANIMATIONSET pSet = nullptr;
    pAniCon->GetTrackAnimationSet(0, &pSet);
    double current = desc.Position;
    double period = pSet->GetPeriod();
    //Debug << "position : " << current << '\n';
    //Debug << "period : " << period << '\n';
    //Debug << "periodic position : " << pSet->GetPeriodicPosition(desc.Position);

    if (current + static_cast<double>(calcedDt) >= period)
    {
        m_isFinishedCurrentAnim = true;

        if (!m_finishEvent.empty())
        {
            if (m_finishEvent.front())
            {
                m_finishEvent.front()();
                m_finishEvent.pop_front();
            }
        }
    }
    else
    {
        m_isFinishedCurrentAnim = false;
    }
    pSet->Release();
}

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

void SkinnedMeshController::drawFrame(
    LPD3DXFRAME pFrameBase, 
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    if (!pFrameBase) return;

    auto pMeshContainer = pFrameBase->pMeshContainer;
    while (pMeshContainer)
    {
        drawMeshContainer(pMeshContainer, setGlobalVariable);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    drawFrame(pFrameBase->pFrameSibling, setGlobalVariable);
    drawFrame(pFrameBase->pFrameFirstChild, setGlobalVariable);
}

void SkinnedMeshController::drawMeshContainer(
    LPD3DXMESHCONTAINER pMeshContainerBase,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    if (!pMeshContainerBase || !pMeshContainerBase->pSkinInfo) return;

    auto pMeshContainer = static_cast<MeshContainer*>(pMeshContainerBase);
    
    ///**/
    //// TODO
    //D3DXVECTOR3 vCenter(0.0f,0.0f,0.0f);
    //D3DXVec3TransformCoord(&vCenter, &pMeshContainer->pEffectMesh->m_center, &GetTransform()->GetTransformationMatrix());
    //if (!CurrentCamera()()->IsObjectInsideFrustum(vCenter, pMeshContainer->pEffectMesh->m_radius))
    //    return;
    ///**/
    
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

    Shader::Draw(
        pMeshContainer->pEffectMesh->m_effectParams, 
        pMeshContainer->m_pWorkMesh, 
        setGlobalVariable);
}

SkinnedMeshController::SkinnedMeshController(IObject* pOwner)
    : Component(pOwner)
    , m_currentAnimIndex(0)
    , m_currentAnimName("")
    , m_totalBlendTime(0.3f)
    , m_passedBlendTime(0.0f)
    , m_pSkinnedMeshInstance(nullptr)
    , m_isFinishedCurrentAnim(false)
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

    const float factor = 2.0f;
    const float dt = Time()()->GetDeltaTime();
    const float calcedDt = dt * factor;

    checkIsFinishedAndSetNextAnimation(calcedDt);

    auto& pAniCon = m_pSkinnedMeshInstance->m_pAnimController;
    pAniCon->AdvanceTime(calcedDt, nullptr);

    // for blend
    if (m_passedBlendTime < m_totalBlendTime)
    {
        m_passedBlendTime += calcedDt;
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

void SkinnedMeshController::Render(
    const std::function<void(LPD3DXEFFECT)>& shaderGlobalSetup)
{
    assert(m_pSkinnedMeshInstance &&
        "SkinnedMeshController::Render(), skinned mesh is null.");

    drawFrame(m_pSkinnedMeshInstance->pSkinnedMesh->m_pRootFrame, shaderGlobalSetup);
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
    const string& name,
    const float nextSpeed,
    const bool isBlend,
    const float blendTime,
    const float nextWeight)
{
    LPD3DXANIMATIONSET pNext = nullptr;
    auto& pAniCon = m_pSkinnedMeshInstance->m_pAnimController;
    cout << name << '\n';
    HRESULT hr = pAniCon->GetAnimationSetByName(name.c_str(), &pNext);

    assert(!FAILED(hr) &&
        "SkinnedMeshController::SetAnimationIndex(), \
         ID3DXAnimationController::GetAnimationSet() failed.");

    m_currentAnimName = name;

    if (isBlend)
    {
        LPD3DXANIMATIONSET pCurrent = nullptr;
        pAniCon->GetTrackAnimationSet(0, &pCurrent);
        pAniCon->SetTrackAnimationSet(1, pCurrent);

        D3DXTRACK_DESC desc;
        pAniCon->GetTrackDesc(0, &desc);
        pAniCon->SetTrackDesc(1, &desc);

        pAniCon->SetTrackWeight(0, nextWeight);
        pAniCon->SetTrackWeight(1, 1.0f - nextWeight);

        SAFE_RELEASE(pCurrent);

        m_passedBlendTime = 0.0f;
        m_totalBlendTime = blendTime;
    }

    pAniCon->SetTrackAnimationSet(0, pNext);
    //pAniCon->ResetTime();
    pAniCon->SetTrackPosition(0, 0.0);
    pAniCon->SetTrackSpeed(0, nextSpeed);

    SAFE_RELEASE(pNext);
}

void SkinnedMeshController::SetAnimation(
    const string& name, 
    const float nextSpeed, 
    const bool isBlend, 
    const float blendTime, 
    const float nextWeight, 
    const std::function<void()>& finishEvent)
{
    SetAnimation(name, nextSpeed, isBlend, blendTime, nextWeight);
    m_finishEvent.emplace_back(finishEvent);
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

bool SkinnedMeshController::IsFinishedCurrentAnim() const
{
    return m_isFinishedCurrentAnim;
}

bool SkinnedMeshController::HasFinishEvent() const
{
    return !m_finishEvent.empty();
}
