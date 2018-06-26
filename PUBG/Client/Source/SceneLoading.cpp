#include "stdafx.h"
#include "SceneLoading.h"
#include "Church.h"
#include "ResPathFileName.h"

SceneLoading::SceneLoading()
    : IScene()
    , m_isFinished(false)
    , m_numAddedAnim(0)
{
}

SceneLoading::~SceneLoading()
{
}

void SceneLoading::OnInit()
{
    //Resource()()->LoadAll();

    pair<string, string> p = ResPathFileName::Get(TAG_RES_STATIC::Church);
    cout << p.first << ", " << p.second << endl;
    //const string churchPath = "./Resource_Temp/Church/";
    //const string churchFilename = "Church.x";
    const string churchPath = p.first;
    const string churchFilename = p.second;

    //ResourceContainer* result = OnLoadEffectMeshAsync(
    //    churchPath, churchFilename);
    //if (result == nullptr)
    //{
    //    cout << "no...\n";
    //}
    //else
    //{
    //    cout << "yes\n";
    //}

    m_start = std::chrono::system_clock::now();

    loadEffectMesh();
    loadSkinnedMesh();
    loadAnimation();
}

void SceneLoading::OnUpdate()
{
    if (m_isFinished)
    {

    }
    else
    {
        if (!m_isDoneCharacters)
        {
            if (verifyTasks(
                &m_characterSkinnedMeshTasks,
                &m_characterSkinnedMeshResources) &&
                verifyTasks(
                    &m_characterAnimationTasks,
                    &m_characterAnimationResources))
            {
                addAnimationsToCharacter0();
            }
        }

        if (!m_isDoneEffectMeshs)
        {
            if (verifyTasks(&m_effectMeshTasks, &m_effectMeshResources))
                addEffectMeshs();
        }

        if (m_isDoneEffectMeshs &&
            m_isDoneCharacters)
        {
            m_isFinished = true;
            m_finish = std::chrono::system_clock::now();
            m_elapsed = m_finish - m_start;
        }
    }
}

void SceneLoading::loadEffectMesh()
{
    addTask(TAG_RESOURCE_STATIC::IDLE, &m_effectMeshTasks);

    // ...
}

void SceneLoading::loadSkinnedMesh()
{
    // Unarmed_Landing.X
    // 이 엑스파일이 애니메이션을 갖고 있는 애들 중 제일 작다

    for (int i = 0; i < Character::NUM_PLAYER; ++i)
        addTask(TAG_RESOURCE_ANIM::IDLE, &m_characterSkinnedMeshTasks);
}

void SceneLoading::loadAnimation()
{
    addTask(TAG_RESOURCE_ANIM::IDLE, &m_characterAnimationTasks);

    // ...
}

void SceneLoading::addAnimationsToCharacter0()
{
    ResourceContainer* pCharacter0 = m_characterAnimationResources.front();

    LPD3DXANIMATIONCONTROLLER& pOld = 
        pCharacter0->m_pSkinnedMesh->m_pAnimController;

    LPD3DXANIMATIONCONTROLLER pAdd    = nullptr;
    LPD3DXANIMATIONCONTROLLER pNew    = nullptr;
    LPD3DXANIMATIONSET        pAddSet = nullptr;
    
    UINT numAddSet = 0;

    for (auto pR : m_characterAnimationResources)
    {
        pAdd = pR->m_pSkinnedMesh->m_pAnimController;

        pOld->CloneAnimationController(
            pOld->GetMaxNumAnimationOutputs(),
            pOld->GetMaxNumAnimationSets() + pAdd->GetMaxNumAnimationSets(),
            pOld->GetMaxNumTracks(),
            pOld->GetMaxNumEvents(),
            &pNew);
        
        numAddSet = pAdd->GetNumAnimationSets();
        for (UINT i = 0; i < numAddSet; ++i)
        {
            pAdd->GetAnimationSet(i, &pAddSet);
            pNew->RegisterAnimationSet(pAddSet);
            pAddSet->Release();

            ++m_numAddedAnim;
        }

        pOld->Release();
        pOld = pNew;

        SAFE_DELETE(pR);
    }
    
    copyAnimationsToOtherCharacters();
}

void SceneLoading::copyAnimationsToOtherCharacters()
{
    ResourceContainer* pCharacter0 = m_characterAnimationResources.front();

    LPD3DXANIMATIONCONTROLLER pC0AniCon = 
        pCharacter0->m_pSkinnedMesh->m_pAnimController;

    LPD3DXANIMATIONCONTROLLER pNew    = nullptr;
    LPD3DXANIMATIONSET        pAddSet = nullptr;

    auto begin = std::next(m_characterAnimationResources.begin());
    auto end   = m_characterAnimationResources.end();
    for (auto i = begin; i != end; ++i)
    {
        LPD3DXANIMATIONCONTROLLER& pOld =
            (*i)->m_pSkinnedMesh->m_pAnimController;

        UINT numDefaultAnim = pOld->GetMaxNumAnimationSets();

        pOld->CloneAnimationController(
            pOld->GetMaxNumAnimationOutputs(),
            pOld->GetMaxNumAnimationSets() + m_numAddedAnim,
            pOld->GetMaxNumTracks(),
            pOld->GetMaxNumEvents(),
            &pNew);

        for (UINT i = numDefaultAnim; i < numDefaultAnim + m_numAddedAnim; ++i)
        {
            pC0AniCon->GetAnimationSet(i, &pAddSet);
            pNew->RegisterAnimationSet(pAddSet);
            pAddSet->Release();
        }

        pOld->Release();
        pOld = pNew;
    }

    Resource()()->AddCharacters(&m_characterSkinnedMeshResources);

    m_isDoneCharacters = true;
}

void SceneLoading::addEffectMeshs()
{
    ResourceManager* pRM = Resource()();

    for (auto pR : m_effectMeshResources)
        pRM->AddResource(pR);

    m_isDoneEffectMeshs = true;
}

bool SceneLoading::verifyTasks(
    std::deque<std::future<ResourceContainer*>>* OutTasks, 
    std::vector<ResourceContainer*>* OutResources)
{
    assert(OutTasks && OutResources && 
        "SceneLoading::verifyTasks(), tasks or resources is null.");

    if (OutTasks->empty()) return true;

    std::future_status futureStatus;
    const auto& begin = OutTasks->begin();
    const auto& end = OutTasks->end();
    for (auto i = begin; i != end;)
    {
        futureStatus = i->wait_for(std::chrono::nanoseconds(1));
        switch (futureStatus)
        {
        case std::future_status::deferred:
        case std::future_status::timeout:
            {
                // waiting...
                ++i;
            }
            break;
        case std::future_status::ready:
            {
                OutResources->emplace_back(i->get());
                i = OutTasks->erase(i);
            }
            break;
        }
    }

    if (OutTasks->empty()) return true;

    return false;
}

void SceneLoading::addTask(
    const TAG_RESOURCE_STATIC tag, 
    std::deque<std::future<ResourceContainer*>>* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = tempfunc(tag);
    OutTasks->emplace_back(
        std::async(
            std::launch::async, &ResourceAsync::OnLoadEffectMeshAsync, 
            keys.first, keys.second));

    ++m_numTotalTasks;
}

void SceneLoading::addTask(
    const TAG_RESOURCE_ANIM tag, 
    std::deque<std::future<ResourceContainer*>>* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = tempfunc(tag);
    OutTasks->emplace_back(
        std::async(
            std::launch::async, &ResourceAsync::OnLoadSkinnedMeshAsync, 
            keys.first, keys.second));

    ++m_numTotalTasks;
}

pair<string, string> SceneLoading::tempfunc(TAG_RESOURCE_STATIC tag)
{
    return pair<string, string>();
}

pair<string, string> SceneLoading::tempfunc(TAG_RESOURCE_ANIM tag)
{
    return pair<string, string>();
}
