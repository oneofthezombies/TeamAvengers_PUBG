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

    //auto numBones = pMeshContainer->pSkinInfo->GetNumBones();
    //for (auto i = 0u; i < numBones; ++i)
    //{
    //    pMeshContainer->m_pFinalBoneMatrices[i] 
    //        = pMeshContainer->m_pBoneOffsetMatrices[i] 
    //        * *pMeshContainer->m_ppBoneMatrixPtrs[i];
    //}

    EffectMesh* pEffectMesh = pMeshContainer->pEffectMesh;
        
    //PBYTE pVerticesSrc = nullptr;
    //pEffectMesh->m_pMesh->LockVertexBuffer(
    //    D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);

    //assert(pVerticesSrc && 
    //    "SkinnedMeshController::drawMeshContainer(), \
    //     source vertices is null.");

    //PBYTE pVerticesDest = nullptr;
    //pMeshContainer->m_pWorkMesh->LockVertexBuffer(0, (LPVOID*)&pVerticesDest);

    //assert(pVerticesDest &&         
    //    "SkinnedMeshController::drawMeshContainer(), \
    //     destination vertices is null.");

    //pMeshContainer->pSkinInfo->UpdateSkinnedMesh(
    //    pMeshContainer->m_pFinalBoneMatrices, nullptr, 
    //    pVerticesSrc, pVerticesDest);

    //pEffectMesh->m_pMesh->UnlockVertexBuffer();
    //pMeshContainer->m_pWorkMesh->UnlockVertexBuffer();

    Shader::Draw(
        pEffectMesh->m_effectParams, 
        pMeshContainer->m_pWorkMesh, 
        setGlobalVariable);
}

void SkinnedMeshController::updateAnimation(
    const float dt, 
    const float totalBlendingTime, 
    LPD3DXANIMATIONCONTROLLER pController, 
    float* OutPassedBlendingTime)
{
    if (!pController) return;

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
    if (!pController) return;

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

    IObject* pParent = GetOwner()->GetParent();
    if (pParent)
    {
        if (static_cast<Character*>(pParent)->GetIndex() == 
            Communication()()->m_myInfo.ID)
        {
            Debug << "track 0\n"
                << "animation name    : " << name << '\n'
                << "current  position : " << currentPosition << '\n'
                << "periodic position : " << periodicPosition << '\n'
                << "period            : " << period << '\n';
        }
    }

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

        IObject* pParent = GetOwner()->GetParent();
        if (pParent)
        {
            if (static_cast<Character*>(pParent)->GetIndex() == 
                Communication()()->m_myInfo.ID)
            {
                Debug << "track 1\n"
                    << "animation name    : " << track1Name << '\n'
                    << "current  position : " << track1CurrentPosition << '\n'
                    << "periodic position : " << track1PeriodicPosition << '\n'
                    << "period            : " << track1Period << "\n\n";
            }
        }

    }
    else
    {
        IObject* pParent = GetOwner()->GetParent();
        if (pParent)
        {
            if (static_cast<Character*>(pParent)->GetIndex() == 
                Communication()()->m_myInfo.ID)
            {
                Debug << "track 1\n"
                    << "animation name    : " << '\n'
                    << "current  position : " << '\n'
                    << "periodic position : " << '\n'
                    << "period            : " << "\n\n";
            }
        }
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
    , pSkinnedMesh(nullptr)
{
}

SkinnedMeshController::~SkinnedMeshController()
{
}

void SkinnedMeshController::UpdateAnimation()
{
    assert(pSkinnedMesh &&
        "SkinnedMeshController::UpdateAnimation(), \
         skinned mesh instance is null.");

    const float factor = 2.0f;
    const float dt = Time()()->GetDeltaTime();
    const float calcedDT = dt * factor;

    notifyAnimationEvent(
        calcedDT,
        m_animName,
        pSkinnedMesh->m_pAnimController,
        &m_loopEvents,
        &m_finishEvents);

    updateAnimation(
        calcedDT,
        m_totalBlendingTime,
        pSkinnedMesh->m_pAnimController,
        &m_passedBlendingTime);

    for (auto& a : m_animationBackup)
        a.second = a.first->TransformationMatrix;

    notifyAnimationEvent(
        calcedDT,
        m_subAnimName,
        pSkinnedMesh->m_pSubAnimController,
        &m_subLoopEvents,
        &m_subFinishEvents);

    updateAnimation(
        calcedDT,
        m_subTotalBlendingTime,
        pSkinnedMesh->m_pSubAnimController,
        &m_subPassedBlendingTime);
}

void SkinnedMeshController::UpdateModel()
{
    assert(
        pSkinnedMesh && 
        "SkinnedMeshController::UpdateModel(), \
         skinned mesh is null.");

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

    UpdateMesh();
}

void SkinnedMeshController::UpdateMesh()
{
    updateMesh(pSkinnedMesh->m_pRootFrame);
    updateMesh(pSkinnedMesh->m_pSubRootFrame);
}

void SkinnedMeshController::Render(
    const D3DXMATRIX& world,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    assert(
        pSkinnedMesh &&
        "SkinnedMeshController::Render(), \
         skinned mesh is null.");

    drawFrame(pSkinnedMesh->m_pRootFrame, world, setGlobalVariable);

    // 현재 사용하는 스킨드 메시들은 메시 컨테이너가 루트로부터만 있음
    //if (pSkinnedMesh->m_pSubRootFrame)
    //    drawFrame(pSkinnedMesh->m_pSubRootFrame, setGlobalVariable);
}

void SkinnedMeshController::SetSkinnedMesh(SkinnedMesh* pSkinnedMesh)
{
    assert(pSkinnedMesh && 
        "SkinnedMeshController::SetSkinnedMesh(), skinned mesh is null.");

    setEffectMesh(pSkinnedMesh->m_pRootFrame);
    setEffectMesh(pSkinnedMesh->m_pSubRootFrame);

    this->pSkinnedMesh = pSkinnedMesh;
}

void SkinnedMeshController::SetSkinnedMesh(
    const std::pair<std::string, std::string>& fullPath,
    const std::size_t index)
{
    SetSkinnedMesh(Resource()()->GetSkinnedMesh(
        fullPath.first, 
        fullPath.second, index));
}

SkinnedMesh* SkinnedMeshController::GetSkinnedMesh() const
{
    return pSkinnedMesh;
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
        pSkinnedMesh &&
        "SkinnedMeshController::SetAnimation(), skinned mesh is null.");

    LPD3DXANIMATIONSET pNext = nullptr;
    LPD3DXANIMATIONSET pCurrent = nullptr;
    LPD3DXANIMATIONCONTROLLER pController = nullptr;
    D3DXTRACK_DESC desc;
    HRESULT hr = 0;

    if (isSub)
    {
        assert(
            pSkinnedMesh->m_pSubAnimController && 
            "SkinnedMeshController::SetAnimation(), \
             sub animation controller is null.");

        m_subAnimName = name;
        pController = pSkinnedMesh->m_pSubAnimController;
    }
    else
    {
        assert(
            pSkinnedMesh->m_pAnimController &&
            "SkinnedMeshController::SetAnimation(), \
             animation controller is null.");

        m_animName = name;
        pController = pSkinnedMesh->m_pAnimController;
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
        pSkinnedMesh && 
        pSkinnedMesh->m_pAnimController &&
        "SkinnedMeshController::GetNumAnimation(), \
         skinned mesh instance or controller is null.");
    
    return pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets();
}

const std::string& SkinnedMeshController::GetSubAnimationName() const
{
    return m_subAnimName;
}

std::size_t SkinnedMeshController::GetSubNumAnimation() const
{
    assert(
        pSkinnedMesh &&
        pSkinnedMesh->m_pSubAnimController &&
        "SkinnedMeshController::GetNumAnimation(), \
         skinned mesh instance or sub controller is null.");

    return pSkinnedMesh
        -> m_pSubAnimController
        -> GetMaxNumAnimationSets();
}

Frame* SkinnedMeshController::FindFrame(const string& name)
{
    assert(pSkinnedMesh &&
        "SkinnedMeshController::FindFrame(), skinned mesh instance is null.");

    LPD3DXFRAME pFrame = 
        D3DXFrameFind(
            pSkinnedMesh->m_pRootFrame, 
            name.c_str());

    if (!pFrame)
    {
        pFrame = 
            D3DXFrameFind(
                pSkinnedMesh->m_pSubRootFrame, 
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
        pSkinnedMesh &&
        pSkinnedMesh->m_pAnimController &&
        "SkinnedMeshController::GetTrackDescription(), \
         skinned mesh instance or animation controller or argument is null.");

    HRESULT hr = 
        pSkinnedMesh->m_pAnimController->GetTrackDesc(
            static_cast<UINT>(index), 
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
        pSkinnedMesh &&
        pSkinnedMesh->m_pSubAnimController &&
        "SkinnedMeshController::GetSubTrackDescription(), \
         skinned mesh instance or animation controller or argument is null.");

    HRESULT hr =
        pSkinnedMesh->m_pSubAnimController->GetTrackDesc(
            static_cast<UINT>(index), 
            OutDesc);

    assert(
        !FAILED(hr) &&
        "SkinnedMeshController::GetSubTrackDescription(), \
         ID3DXAnimationController::GetTrackDesc() failed.");
}

float SkinnedMeshController::GetTrackPeriod(const std::size_t index)
{
    LPD3DXANIMATIONSET pSet = nullptr;
    pSkinnedMesh->m_pAnimController->GetTrackAnimationSet(static_cast<UINT>(index), &pSet);
    float period = static_cast<float>(pSet->GetPeriod());
    pSet->Release();
    return period;
}

float SkinnedMeshController::GetSubTrackPeriod(const std::size_t index)
{
    LPD3DXANIMATIONSET pSet = nullptr;
    pSkinnedMesh->m_pSubAnimController->GetTrackAnimationSet(static_cast<UINT>(index), &pSet);
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
    EffectMesh* pEffectMesh = pMC->pEffectMesh;

    BoundingSphere bs;
    bs.center = pEffectMesh->m_boundingSphere.center;
    bs.radius = pEffectMesh->m_boundingSphere.radius;
    OutBoundingSpheres->emplace_back(bs);
}

//void SkinnedMeshController::findMesh(
//    LPD3DXFRAME pFrame, 
//    std::vector<std::pair<LPD3DXMESH, std::size_t>>* OutMeshs)
//{
//    if (!pFrame) return;
//
//    findMesh(pFrame->pMeshContainer, OutMeshs);
//
//    findMesh(pFrame->pFrameSibling, OutMeshs);
//    findMesh(pFrame->pFrameFirstChild, OutMeshs);
//}
//
//void SkinnedMeshController::findMesh(
//    LPD3DXMESHCONTAINER pMeshContainer, 
//    std::vector<std::pair<LPD3DXMESH, std::size_t>>* OutMeshs)
//{
//    if (!pMeshContainer) return;
//
//    MeshContainer* pMC = static_cast<MeshContainer*>(pMeshContainer);
//    EffectMesh* pEffectMesh = pMC->pEffectMesh;
//
//    OutMeshs->emplace_back(
//        std::make_pair(
//            pEffectMesh->m_pMesh, 
//            pEffectMesh->m_effectParams.size()));
//}

void SkinnedMeshController::setEffectMesh(LPD3DXFRAME pFrame)
{
    if (!pFrame) return;

    LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
    while (pMeshContainer)
    {
        setEffectMesh(pMeshContainer);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    setEffectMesh(pFrame->pFrameSibling);
    setEffectMesh(pFrame->pFrameFirstChild);
}

void SkinnedMeshController::setEffectMesh(LPD3DXMESHCONTAINER pMeshContainer)
{
    if (!pMeshContainer) return;

    MeshContainer* pMC = static_cast<MeshContainer*>(pMeshContainer);
    pMC->pEffectMesh = Resource()()->GetEffectMesh(pMC->m_effectMeshKey);

    assert(
        pMC->pEffectMesh && 
        "SkinnedMeshController::setEffectMesh() effect mesh is null.");
}

void SkinnedMeshController::updateMesh(LPD3DXFRAME pFrame)
{
    if (!pFrame) return;

    LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
    while (pMeshContainer)
    {
        updateMesh(pMeshContainer);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    updateMesh(pFrame->pFrameSibling);
    updateMesh(pFrame->pFrameFirstChild);
}

void SkinnedMeshController::updateMesh(LPD3DXMESHCONTAINER pMeshContainer)
{
    if (!pMeshContainer || !pMeshContainer->pSkinInfo) return;

    auto pMC = static_cast<MeshContainer*>(pMeshContainer);

    auto numBones = pMC->pSkinInfo->GetNumBones();
    for (auto i = 0u; i < numBones; ++i)
    {
        pMC->m_pFinalBoneMatrices[i]
            =  pMC->m_pBoneOffsetMatrices[i]
            * *pMC->m_ppBoneMatrixPtrs[i];
    }

    EffectMesh* pEffectMesh = pMC->pEffectMesh;

    PBYTE pVerticesSrc = nullptr;
    pEffectMesh->m_pMesh->LockVertexBuffer(
        D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);

    assert(pVerticesSrc &&
        "SkinnedMeshController::drawMeshContainer(), \
         source vertices is null.");

    PBYTE pVerticesDest = nullptr;
    pMC->m_pWorkMesh->LockVertexBuffer(0, (LPVOID*)&pVerticesDest);

    assert(pVerticesDest &&
        "SkinnedMeshController::drawMeshContainer(), \
         destination vertices is null.");

    pMC->pSkinInfo->UpdateSkinnedMesh(
        pMC->m_pFinalBoneMatrices, nullptr,
        pVerticesSrc, pVerticesDest);

    pEffectMesh->m_pMesh->UnlockVertexBuffer();
    pMC->m_pWorkMesh->UnlockVertexBuffer();
}

std::vector<BoundingSphere> SkinnedMeshController::GetBoundingSpheres()
{
    std::vector<BoundingSphere> boundingSpheres;

    findBoundingSphere(
        pSkinnedMesh->m_pRootFrame, 
        &boundingSpheres);

    findBoundingSphere(
        pSkinnedMesh->m_pSubRootFrame, 
        &boundingSpheres);

    return boundingSpheres;
}

void SkinnedMeshController::SetTrackPosition(const std::size_t index, const double position)
{
    assert(
        pSkinnedMesh && 
        pSkinnedMesh->m_pAnimController && 
        "SkinnedMeshController::SetPosition(), skinned mesh or anim controller is null.");

    pSkinnedMesh->m_pAnimController->SetTrackPosition(static_cast<UINT>(index), position);
}

void SkinnedMeshController::SetSubTrackPosition(const std::size_t index, const double position)
{
    assert(
        pSkinnedMesh &&
        pSkinnedMesh->m_pAnimController &&
        "SkinnedMeshController::SetSubPosition(), skinned mesh or anim controller is null.");

    pSkinnedMesh->m_pSubAnimController->SetTrackPosition(static_cast<UINT>(index), position);
}
