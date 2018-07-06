#include "stdafx.h"
#include "SkinnedMeshController.h"

void SkinnedMeshController::updateFrameToModelSpace(LPD3DXFRAME pFrameBase,
    LPD3DXFRAME pParent)
{
    if (!pFrameBase) return;

    //if (pFrameBase->Name)
    //    cout << pFrameBase->Name << endl;

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
    const D3DXMATRIX& world,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    if (!pFrameBase) return;

    auto pMeshContainer = pFrameBase->pMeshContainer;
    while (pMeshContainer)
    {
        drawMeshContainer(pMeshContainer, world, setGlobalVariable);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    drawFrame(pFrameBase->pFrameSibling, world, setGlobalVariable);
    drawFrame(pFrameBase->pFrameFirstChild, world, setGlobalVariable);
}

void SkinnedMeshController::drawMeshContainer(
    LPD3DXMESHCONTAINER pMeshContainerBase,
    const D3DXMATRIX& world,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    if (!pMeshContainerBase || !pMeshContainerBase->pSkinInfo) return;

    auto pMeshContainer = static_cast<MeshContainer*>(pMeshContainerBase);
    

    //**************frustum culling ************************
    D3DXVECTOR3 center = Vector3::ZERO;
    D3DXVec3TransformCoord(&center, &pMeshContainer->pEffectMesh->m_center, &world);
    if (!CurrentCamera()()->IsObjectInsideFrustum(center, pMeshContainer->pEffectMesh->m_radius))
        return;
    //******************************************************


    ////sphere around player
    //Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    //Shader::Draw(
    //    Resource()()->GetEffect("./Resource/", "Color.fx"),
    //    nullptr,
    //    m_testmeshSphere,
    //    0,
    //    [this, &world](LPD3DXEFFECT pEffect)
    //{
    //    pEffect->SetMatrix(Shader::World, &world);
    //    D3DXCOLOR Green(0.0f, 1.0f, 0.0f, 1.0f);
    //    pEffect->SetValue("Color", &Green, sizeof Green);
    //});
    //Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);



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

void SkinnedMeshController::updateAnimation(
    const float dt, 
    const float totalBlendingTime, 
    LPD3DXANIMATIONCONTROLLER pController, 
    float* OutPassedBlendingTime)
{
    assert(
        pController && 
        OutPassedBlendingTime &&
        "SkinnedMeshController::updateAnimation(), argument is null.");

    pController->AdvanceTime(dt, nullptr);

    if (*OutPassedBlendingTime < totalBlendingTime)
    {
        *OutPassedBlendingTime += dt;
        const float weight = *OutPassedBlendingTime / totalBlendingTime;
        if (weight >= 1.0f)
        {
            pController->SetTrackWeight(0, 1.0f);
            pController->SetTrackWeight(1, 0.0f);
            pController->SetTrackEnable(1, false);
        }
        else
        {
            pController->SetTrackWeight(0, weight);
            pController->SetTrackWeight(1, 1.0f - weight);
        }
    }
}

void SkinnedMeshController::notifyAnimationEvent(
    const float dt, 
    const std::string& name,
    LPD3DXANIMATIONCONTROLLER pController, 
    animation_events_t* OutLoopEvents, 
    animation_events_t* OutFinishEvents)
{
    assert(
        pController && 
        OutLoopEvents && 
        OutFinishEvents && 
        "SkinnedMeshController::notifyAnimationEvent(), argument is null.");

    D3DXTRACK_DESC desc;
    pController->GetTrackDesc(0, &desc);
    LPD3DXANIMATIONSET pSet = nullptr;
    pController->GetTrackAnimationSet(0, &pSet);
    double currentPosition = desc.Position;
    double period = pSet->GetPeriod();
    double periodicPosition = pSet->GetPeriodicPosition(currentPosition);
    pSet->Release();

    Debug << "track 0\n"
          << "animation name    : " << name             << '\n'
          << "current  position : " << currentPosition  << '\n'
          << "periodic position : " << periodicPosition << '\n'
          << "period            : " << period           << '\n';

    pController->GetTrackDesc(1, &desc);
    if (desc.Enable)
    {
        std::string track1Name;
        double track1CurrentPosition, track1PeriodicPosition, track1Period;

        pController->GetTrackAnimationSet(1, &pSet);

        track1Name = std::string(pSet->GetName());
        track1CurrentPosition = desc.Position;
        track1PeriodicPosition = pSet->GetPeriodicPosition(currentPosition);
        track1Period = pSet->GetPeriod();
        pSet->Release();

        Debug << "track 1\n"
              << "animation name    : " << track1Name << '\n'
              << "current  position : " << track1CurrentPosition << '\n'
              << "periodic position : " << track1PeriodicPosition << '\n'
              << "period            : " << track1Period << "\n\n";
    }
    else
    {
        Debug << "track 1\n"
              << "animation name    : " << '\n'
              << "current  position : " << '\n'
              << "periodic position : " << '\n'
              << "period            : " << "\n\n";
    }

    const double dDT = static_cast<double>(dt);
    const double nextPosition = currentPosition + dDT;

    if (!OutFinishEvents->empty())
    {
        float           finishEventAgoTime = OutFinishEvents->front().first;
        std::function<void()>& finishEvent = OutFinishEvents->front().second;

        if (finishEvent)
        {
            if (nextPosition >= (period - finishEventAgoTime))
            {
                finishEvent();
                OutFinishEvents->pop_front();
            }
        }
    }

    if (nextPosition >= period)
    {
        if (!OutLoopEvents->empty())
            OutLoopEvents->pop_front();
    }

    if (!OutLoopEvents->empty())
    {
        auto& periodAndLoopEvent = OutLoopEvents->front();
        const float loopPeriod = periodAndLoopEvent.first;
        std::function<void()>& loopEvent = periodAndLoopEvent.second;

        if (loopEvent)
        {
            float loopPeriodicPosition = static_cast<float>(currentPosition);
            while (loopPeriodicPosition > loopPeriod)
                loopPeriodicPosition -= loopPeriod;

            if (loopPeriodicPosition + dDT >= loopPeriod)
            {
                loopEvent();
            }
        }
    }
}

SkinnedMeshController::SkinnedMeshController(IObject* pOwner)
    : Component(pOwner)
    , m_animName("")
    , m_totalBlendingTime(0.3f)
    , m_passedBlendingTime(0.0f)
    , m_subAnimName("")
    , m_subTotalBlendingTime(0.3f)
    , m_subPassedBlendingTime(0.0f)
    , m_pSkinnedMeshInstance(nullptr)
    , m_testmeshSphere(NULL)
{
    D3DXCreateSphere(Device()(), 96.42f, 5, 5, &m_testmeshSphere, NULL);
}

SkinnedMeshController::~SkinnedMeshController()
{
    SAFE_DELETE(m_pSkinnedMeshInstance);
    SAFE_RELEASE(m_testmeshSphere);
}

void SkinnedMeshController::UpdateAnimation()
{
    assert(m_pSkinnedMeshInstance &&
        "SkinnedMeshController::UpdateAnimation(), \
         skinned mesh instance is null.");

    const float factor = 2.0f;
    const float dt = Time()()->GetDeltaTime();
    const float calcedDT = dt * factor;
    SkinnedMesh* pSkinnedMesh = m_pSkinnedMeshInstance->pSkinnedMesh;

    notifyAnimationEvent(
        calcedDT,
        m_animName,
        m_pSkinnedMeshInstance->m_pAnimController,
        &m_loopEvents,
        &m_finishEvents);

    updateAnimation(
        calcedDT,
        m_totalBlendingTime,
        m_pSkinnedMeshInstance->m_pAnimController,
        &m_passedBlendingTime);

    //FindFrame("root")->TransformationMatrix = Matrix::IDENTITY;
    //updateFrameToModelSpace(pSkinnedMesh->m_pRootFrame, nullptr);

    for (auto& a : m_animationBackup)
        a.second = a.first->TransformationMatrix;

    notifyAnimationEvent(
        calcedDT,
        m_subAnimName,
        m_pSkinnedMeshInstance->m_pSubAnimController,
        &m_subLoopEvents,
        &m_subFinishEvents);

    updateAnimation(
        calcedDT,
        m_subTotalBlendingTime,
        m_pSkinnedMeshInstance->m_pSubAnimController,
        &m_subPassedBlendingTime);

    //FindFrame("root")->TransformationMatrix = Matrix::IDENTITY;
}

void SkinnedMeshController::UpdateModel()
{
    assert(
        m_pSkinnedMeshInstance && 
        m_pSkinnedMeshInstance->pSkinnedMesh && 
        "SkinnedMeshController::UpdateModel(), \
         skinned mesh instance or skinned mehs is null.");

    SkinnedMesh* pSkinnedMesh = m_pSkinnedMeshInstance->pSkinnedMesh;

    for (auto& a : m_animationBackup)
        a.first->TransformationMatrix = a.second;

    updateFrameToModelSpace(
        pSkinnedMesh->m_pRootFrame,
        nullptr);

    if (pSkinnedMesh->m_pSubRootFrame)
    {
        updateFrameToModelSpace(
            pSkinnedMesh->m_pSubRootFrame,
            pSkinnedMesh->pConnectFrame);
    }
}

void SkinnedMeshController::Render(
    const D3DXMATRIX& world,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    assert(
        m_pSkinnedMeshInstance &&
        m_pSkinnedMeshInstance->pSkinnedMesh &&
        "SkinnedMeshController::Render(), \
         skinned mesh instance or skinned mesh is null.");

    SkinnedMesh* pSkinnedMesh = m_pSkinnedMeshInstance->pSkinnedMesh;
    drawFrame(pSkinnedMesh->m_pRootFrame, world, setGlobalVariable);

    //if (pSkinnedMesh->m_pSubRootFrame)
    //    drawFrame(pSkinnedMesh->m_pSubRootFrame, setGlobalVariable);
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
    
    if (pSkinnedMesh->m_pSubRootFrame)
    {
        pSrc->CloneAnimationController(
            pSrc->GetMaxNumAnimationOutputs(),
            pSrc->GetMaxNumAnimationSets(),
            pSrc->GetMaxNumTracks(),
            pSrc->GetMaxNumEvents(),
            &pDest);

        m_pSkinnedMeshInstance->m_pSubAnimController = pDest;
    }
}

void SkinnedMeshController::SetSkinnedMesh(
    const std::pair<std::string, std::string>& fullPath)
{
    SetSkinnedMesh(Resource()()->GetSkinnedMesh(
        fullPath.first, 
        fullPath.second));
}

void SkinnedMeshController::SetAnimation(
    const bool isSub, 
    const std::string& name, 
    const float nextSpeed, 
    const bool isBlend, 
    const float blendingTime, 
    const float nextWeight,
    const float position)
{
    assert(
        m_pSkinnedMeshInstance &&
        "SkinnedMeshController::SetAnimation(), skinned mesh is null.");

    LPD3DXANIMATIONSET pNext = nullptr;
    LPD3DXANIMATIONSET pCurrent = nullptr;
    LPD3DXANIMATIONCONTROLLER pController = nullptr;
    D3DXTRACK_DESC desc;
    HRESULT hr = 0;

    if (isSub)
    {
        assert(
            m_pSkinnedMeshInstance->m_pSubAnimController && 
            "SkinnedMeshController::SetAnimation(), \
             sub animation controller is null.");

        m_subAnimName = name;
        pController = m_pSkinnedMeshInstance->m_pSubAnimController;
    }
    else
    {
        assert(
            m_pSkinnedMeshInstance->m_pAnimController &&
            "SkinnedMeshController::SetAnimation(), \
             animation controller is null.");

        m_animName = name;
        pController = m_pSkinnedMeshInstance->m_pAnimController;
    }

    hr = pController->GetAnimationSetByName(name.c_str(), &pNext);
    if (FAILED(hr))
    {
        std::string text(
            "HRESULT : "
            + std::to_string(hr)
            + ", SkinnedMeshController::SetAnimation(), \
                   ID3DXAnimationController::GetAnimationSetByName() failed.");
        MessageBoxA(nullptr, text.c_str(), nullptr, MB_OK);
    }

    pController->GetTrackAnimationSet(0, &pCurrent);
    pController->GetTrackDesc(0, &desc);

    const float dt = Time()()->GetDeltaTime() * 2.0f;
    const double period = pCurrent->GetPeriod();
    const double nextPosition = desc.Position + static_cast<double>(dt);

    if (isSub)
    {
        if (!m_subFinishEvents.empty())
        {
            const float finishEventAgoTime = 
                m_subFinishEvents.front().first;

            std::function<void()>& finishEvent = 
                m_subFinishEvents.front().second;

            if (nextPosition < (period - finishEventAgoTime))
                m_subFinishEvents.pop_front();
        }
    }
    else
    {
        if (!m_finishEvents.empty())
        {
            const float finishEventAgoTime = 
                m_finishEvents.front().first;

            std::function<void()>& finishEvent = 
                m_finishEvents.front().second;

            if (nextPosition < (period - finishEventAgoTime))
                m_finishEvents.pop_front();
        }
    }

    if (isBlend)
    {
        pController->SetTrackAnimationSet(1, pCurrent);
        pController->SetTrackDesc(1, &desc);

        pController->SetTrackWeight(0, nextWeight);
        pController->SetTrackWeight(1, 1.0f - nextWeight);

        if (isSub)
        {
            m_subPassedBlendingTime = 0.0f;
            m_subTotalBlendingTime = blendingTime;
        }
        else
        {
            m_passedBlendingTime = 0.0f;
            m_totalBlendingTime = blendingTime;
        }
    }

    SAFE_RELEASE(pCurrent);

    pController->SetTrackAnimationSet(0, pNext);
    pController->SetTrackPosition(0, static_cast<double>(position));
    pController->SetTrackSpeed(0, nextSpeed);

    SAFE_RELEASE(pNext);
}

void SkinnedMeshController::SetAnimation(
    const bool isSub, 
    const std::string& name, 
    const float nextSpeed, 
    const bool isBlend, 
    const float blendingTime, 
    const float nextWeight,
    const float position,
    const float finishEventAgoTime, 
    const std::function<void()>& finishEvent)
{
    SetAnimation(
        isSub, 
        name, 
        nextSpeed, 
        isBlend, 
        blendingTime, 
        nextWeight, 
        position);

    const auto event = std::make_pair(finishEventAgoTime, finishEvent);
    if (isSub)
    {
        m_subFinishEvents.emplace_back(event);
    }
    else
    {
        m_finishEvents.emplace_back(event);
    }
}

void SkinnedMeshController::SetAnimation(
    const bool isSub, 
    const std::string& name, 
    const float nextSpeed, 
    const bool isBlend, 
    const float blendingTime, 
    const float nextWeight,
    const float position,
    const float loopEventPeriod, 
    const std::function<void()>& loopEvent, 
    const float finishEventAgoTime, 
    const std::function<void()>& finishEvent)
{
    if (finishEventAgoTime == NO_ANIMATION_FINISH_EVENT)
    {
        SetAnimation(
            isSub,
            name,
            nextSpeed,
            isBlend,
            blendingTime,
            nextWeight,
            position);
    }
    else
    {
        SetAnimation(
            isSub,
            name,
            nextSpeed,
            isBlend,
            blendingTime,
            nextWeight,
            position,
            finishEventAgoTime,
            finishEvent);
    }

    const auto loopPair = std::make_pair(loopEventPeriod, loopEvent);
    if (isSub)
    {
        m_subLoopEvents.emplace_back(loopPair);
    }
    else
    {
        m_loopEvents.emplace_back(loopPair);
    }
}

const std::string& SkinnedMeshController::GetAnimationName() const
{
    return m_animName;
}

std::size_t SkinnedMeshController::GetNumAnimation() const
{
    assert(
        m_pSkinnedMeshInstance && 
        m_pSkinnedMeshInstance->m_pAnimController &&
        "SkinnedMeshController::GetNumAnimation(), \
         skinned mesh instance or controller is null.");
    
    return m_pSkinnedMeshInstance->m_pAnimController->GetMaxNumAnimationSets();
}

const std::string& SkinnedMeshController::GetSubAnimationName() const
{
    return m_subAnimName;
}

std::size_t SkinnedMeshController::GetSubNumAnimation() const
{
    assert(
        m_pSkinnedMeshInstance &&
        m_pSkinnedMeshInstance->m_pSubAnimController &&
        "SkinnedMeshController::GetNumAnimation(), \
         skinned mesh instance or sub controller is null.");

    return m_pSkinnedMeshInstance
        -> m_pSubAnimController
        -> GetMaxNumAnimationSets();
}

Frame* SkinnedMeshController::FindFrame(const string& name)
{
    assert(m_pSkinnedMeshInstance &&
        "SkinnedMeshController::FindFrame(), skinned mesh instance is null.");

    LPD3DXFRAME pFrame = 
        D3DXFrameFind(
            m_pSkinnedMeshInstance->pSkinnedMesh->m_pRootFrame, 
            name.c_str());

    if (!pFrame)
    {
        pFrame = 
            D3DXFrameFind(
                m_pSkinnedMeshInstance->pSkinnedMesh->m_pSubRootFrame, 
                name.c_str());
    }

    assert(pFrame && 
        "SkinnedMeshController::FindFrame(), D3DXFrameFind() failed.");

    return static_cast<Frame*>(pFrame);
}

void SkinnedMeshController::AddAnimationBackupFrame(Frame* pFrame)
{
    assert(pFrame && 
        "SkinnedMeshController::AddAnimationBackupFrame(), frame is null.");

    m_animationBackup.emplace_back(std::make_pair(pFrame, Matrix::IDENTITY));
}

bool SkinnedMeshController::HasFinishEvent() const
{
    return !m_finishEvents.empty();
}

bool SkinnedMeshController::HasSubFinishEvent() const
{
    return !m_subFinishEvents.empty();
}

void SkinnedMeshController::GetTrackDescription(
    const std::size_t index, 
    D3DXTRACK_DESC* OutDesc)
{
    assert(
        OutDesc &&
        m_pSkinnedMeshInstance &&
        m_pSkinnedMeshInstance->m_pAnimController &&
        "SkinnedMeshController::GetTrackDescription(), \
         skinned mesh instance or animation controller or argument is null.");

    HRESULT hr = 
        m_pSkinnedMeshInstance->m_pAnimController->GetTrackDesc(
            index, 
            OutDesc);

    assert(
        !FAILED(hr) && 
        "SkinnedMeshController::GetTrackDescription(), \
         ID3DXAnimationController::GetTrackDesc() failed.");
}

void SkinnedMeshController::GetSubTrackDescription(
    const std::size_t index, 
    D3DXTRACK_DESC* OutDesc)
{
    assert(
        OutDesc &&
        m_pSkinnedMeshInstance &&
        m_pSkinnedMeshInstance->m_pSubAnimController &&
        "SkinnedMeshController::GetSubTrackDescription(), \
         skinned mesh instance or animation controller or argument is null.");

    HRESULT hr =
        m_pSkinnedMeshInstance->m_pSubAnimController->GetTrackDesc(
            index, 
            OutDesc);

    assert(
        !FAILED(hr) &&
        "SkinnedMeshController::GetSubTrackDescription(), \
         ID3DXAnimationController::GetTrackDesc() failed.");
}

float SkinnedMeshController::GetTrackPeriod(const std::size_t index)
{
    LPD3DXANIMATIONSET pSet = nullptr;
    m_pSkinnedMeshInstance->m_pAnimController->GetTrackAnimationSet(index, &pSet);
    float period = static_cast<float>(pSet->GetPeriod());
    pSet->Release();
    return period;
}

float SkinnedMeshController::GetSubTrackPeriod(const std::size_t index)
{
    LPD3DXANIMATIONSET pSet = nullptr;
    m_pSkinnedMeshInstance->m_pSubAnimController->GetTrackAnimationSet(index, &pSet);
    float period = static_cast<float>(pSet->GetPeriod());
    pSet->Release();
    return period;
}

void SkinnedMeshController::findBoundingSphere(
    LPD3DXFRAME pFrame, 
    std::vector<BoundingSphere>* OutBoundingSpheres)
{
    if (!pFrame) return;
  
    findBoundingSphere(pFrame->pMeshContainer, OutBoundingSpheres);

    findBoundingSphere(pFrame->pFrameSibling, OutBoundingSpheres);
    findBoundingSphere(pFrame->pFrameFirstChild, OutBoundingSpheres);
}

void SkinnedMeshController::findBoundingSphere(
    LPD3DXMESHCONTAINER pMeshContainer, 
    std::vector<BoundingSphere>* OutBoundingSpheres)
{
    if (!pMeshContainer) return;

    MeshContainer* pMC = static_cast<MeshContainer*>(pMeshContainer);
    BoundingSphere bs;
    bs.center = pMC->pEffectMesh->m_center;
    bs.radius = pMC->pEffectMesh->m_radius;
    OutBoundingSpheres->emplace_back(bs);
}

std::vector<BoundingSphere> SkinnedMeshController::GetBoundingSpheres()
{
    std::vector<BoundingSphere> boundingSpheres;
    findBoundingSphere(m_pSkinnedMeshInstance->pSkinnedMesh->m_pRootFrame, &boundingSpheres);
    findBoundingSphere(m_pSkinnedMeshInstance->pSkinnedMesh->m_pSubRootFrame, &boundingSpheres);
    return boundingSpheres;
}
