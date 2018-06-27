#include "stdafx.h"
#include "SceneLoading.h"
#include "ResPathFileName.h"
#include "Character.h"
#include "UIText.h"

SceneLoading::SceneLoading()
    : IScene()
    , m_isFinished(false)
    , m_numAddedAnim(0)
    , m_numFinishedTasks(0)
    , m_numTotalTasks(0)
    , m_percentage("")
    , m_lastFinishedTaskName("")
    , m_dotDotDot(0)
{
}

SceneLoading::~SceneLoading()
{
}

void SceneLoading::OnInit()
{
    m_start = std::chrono::system_clock::now();

    loadEffectMesh();
    loadSkinnedMesh();
    loadAnimation();

    m_pPercentageImage = new UIText(Resource()()->GetFont(TAG_FONT::DEFAULT), 
        D3DXVECTOR2(1000.0f, 50.0f), &m_percentage, D3DCOLOR_XRGB(0, 255, 0), 
        nullptr);
    m_pPercentageImage->SetPosition(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
    m_pPercentageImage->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
}

void SceneLoading::OnUpdate()
{
    if (m_isFinished)
    {
        Debug << "elapsed time : " << m_elapsed.count() << '\n';

        //UI()()->Destroy(m_pPercentageImage);
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
            {
                addEffectMeshs();
            }
        }

        if (m_isDoneEffectMeshs &&
            m_isDoneCharacters)
        {
            m_isFinished = true;
            m_finish = std::chrono::system_clock::now();
            m_elapsed = m_finish - m_start;

            AddObject(new Character(0));
        }
    }

    Debug << "number of finished tasks : " << m_numFinishedTasks
          << " / number of total tasks : " << m_numTotalTasks << '\n';

    const float percentage = static_cast<float>(m_numFinishedTasks) 
                           / static_cast<float>(m_numTotalTasks)
                           * 100.0f;

    ++m_dotDotDot;
    if (m_dotDotDot > 100)
        m_dotDotDot = 0;

    m_percentage = "Finished file : " + m_lastFinishedTaskName + "\n";
    for (int i = 0; i < static_cast<int>(percentage * 0.1f); ++i)
        m_percentage += "@";
    m_percentage += " percentage : " + std::to_string(percentage);
    for (std::size_t i = 0; i < m_dotDotDot; ++i)
        m_percentage += '.';
}

void SceneLoading::loadEffectMesh()
{
    addTask(TAG_RES_STATIC::Church, &m_effectMeshTasks);

    // ...
}

void SceneLoading::loadSkinnedMesh()
{
    // Unarmed_Landing.X
    // �� ���������� �ִϸ��̼��� ���� �ִ� �ֵ� �� ���� �۴�

    for (int i = 0; i < Character::NUM_PLAYER; ++i)
        addTask(TAG_RES_ANIM::Unarmed_Landing, &m_characterSkinnedMeshTasks);

    //pair<string, string> p;
    //for (int i = 0; i < static_cast<int>(TAG_RES_ANIM::COUNT); ++i)
    //{
    //    p = ResPathFileName::Get(static_cast<TAG_RES_ANIM>(i));
    //    cout << p.first << p.second << endl;
    //}
}

void SceneLoading::loadAnimation()
{
    addTask(TAG_RES_ANIM::Lobby, &m_characterAnimationTasks);

    // ...
}

void SceneLoading::addAnimationsToCharacter0()
{
    ResourceContainer* pC0 = m_characterSkinnedMeshResources.front();

    LPD3DXANIMATIONCONTROLLER& pOld = pC0->m_pSkinnedMesh->m_pAnimController;

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
    ResourceContainer* pC0 = m_characterSkinnedMeshResources.front();

    LPD3DXANIMATIONCONTROLLER pC0AniCon = 
        pC0->m_pSkinnedMesh->m_pAnimController;

    LPD3DXANIMATIONCONTROLLER pNew    = nullptr;
    LPD3DXANIMATIONSET        pAddSet = nullptr;

    for (auto i = std::next(m_characterAnimationResources.begin()); 
              i != m_characterAnimationResources.end(); 
            ++i)
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
    for (auto i = OutTasks->begin(); i != OutTasks->end();)
    {
        //futureStatus = i->wait_for(std::chrono::nanoseconds(1));
        futureStatus = i->wait_until(std::chrono::system_clock::now());
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
                ResourceContainer* pResourceContainer = i->get();
                OutResources->emplace_back(pResourceContainer);
                i = OutTasks->erase(i);

                m_lastFinishedTaskName = pResourceContainer->m_filename;
                ++m_numFinishedTasks;
            }
            break;
        }
    }

    if (OutTasks->empty()) return true;

    return false;
}

void SceneLoading::addTask(
    const TAG_RES_STATIC tag, 
    std::deque<std::future<ResourceContainer*>>* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = ResPathFileName::Get(tag);
    OutTasks->emplace_back(
        std::async(
            std::launch::async, &ResourceAsync::OnLoadEffectMeshAsync, 
            keys.first, keys.second));

    ++m_numTotalTasks;
}

void SceneLoading::addTask(
    const TAG_RES_ANIM tag, 
    std::deque<std::future<ResourceContainer*>>* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = ResPathFileName::Get(tag);
    OutTasks->emplace_back(
        std::async(
            std::launch::async, &ResourceAsync::OnLoadSkinnedMeshAsync, 
            keys.first, keys.second));

    ++m_numTotalTasks;
}