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

    m_pPercentageImage = new UIText(Resource()()->GetFont(TAG_FONT::Default), 
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

        UI()()->Destroy(m_pPercentageImage);
        Scene()()->SetCurrentScene(TAG_SCENE::Play);
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
                addAnimationsToCharacter();
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
    addTask(TAG_RES_STATIC::Bandage);
    //addTask(TAG_RES_STATIC::Church);


    // ...
}

void SceneLoading::loadSkinnedMesh()
{
    // Rifle_Prone_PrimarySlot_OnHand.X
    // 이 엑스파일이 애니메이션을 갖고 있는 애들 중 제일 작다 (애니메이션 1개)
    /* now, all characters use the same skinned mesh, 
       so we do not need to have duplicates.
    */

    /* do NOT remove &m_characterSkinnedMeshTasks, 
       single argument function is for animaiton. */
    addTask(TAG_RES_ANIM_CHARACTER::Lobby, &m_characterSkinnedMeshTasks);

    loadCharacterAnimation();
}

void SceneLoading::loadCharacterAnimation()
{
    addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Idling);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Attack);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Attack_FPP);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_DoorOpen_And_Pickup);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Pickup_FPP);
    addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Jump);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Jump_FPP);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Landing);
    addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion_Stand);

    //addTask(TAG_RES_ANIM_CHARACTER::Rifle_Idling);
    //addTask(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Prone);
    ////addTask(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Stand); // has bug
    //addTask(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Crouch);
    //addTask(TAG_RES_ANIM_CHARACTER::Rifle_OnBody);


    /*
    //아직 두개 이상 들어가는게 안되네여????
    pair<string, string> p;
    for (int i = 0; i < static_cast<int>(TAG_RES_ANIM_CHARACTER::COUNT); ++i)
    {
        p = ResPathFileName::Get(static_cast<TAG_RES_ANIM_CHARACTER>(i));
        cout << p.second << endl;
        addTask(static_cast<TAG_RES_ANIM_CHARACTER>(i), &m_characterAnimationTasks); //enum 순서대로 x파일이 들어감
    }
    */
    // ...
}

void SceneLoading::addAnimationsToCharacter()
{
    Resource()()->AddCharacter(
        m_characterSkinnedMeshResources.begin()->second);
    SkinnedMesh* pCharacter = Resource()()->GetCharacterSkinnedMesh();

    LPD3DXANIMATIONCONTROLLER& pOld = pCharacter->m_pAnimController;

    LPD3DXANIMATIONCONTROLLER pAdd    = nullptr;
    LPD3DXANIMATIONCONTROLLER pNew    = nullptr;
    LPD3DXANIMATIONSET        pAddSet = nullptr;
    
    UINT numAddSet = 0;

    for (auto pR : m_characterAnimationResources)
    {
        pAdd = pR.second->m_pSkinnedMesh.second->m_pAnimController;

        pOld->CloneAnimationController(
            pOld->GetMaxNumAnimationOutputs(),
            pOld->GetMaxNumAnimationSets() + pAdd->GetMaxNumAnimationSets(),
            pOld->GetMaxNumTracks(),
            pOld->GetMaxNumEvents(),
            &pNew);
        
        numAddSet = pAdd->GetNumAnimationSets();
        for (int i = static_cast<int>(numAddSet) - 1; i >= 0; --i)
        {
            pAdd->GetAnimationSet(i, &pAddSet);
            pNew->RegisterAnimationSet(pAddSet);
            pAddSet->Release();

            ++m_numAddedAnim;
        }

        pOld->Release();
        pOld = pNew;

        SAFE_DELETE(pR.second);
    }

    m_isDoneCharacters = true;
}

//void SceneLoading::copyAnimationsToOtherCharacters()
//{
//    ResourceContainer* pC0 = m_characterSkinnedMeshResources.begin()->second;
//
//    LPD3DXANIMATIONCONTROLLER pC0AniCon = 
//        pC0->m_pSkinnedMesh->m_pAnimController;
//
//    LPD3DXANIMATIONCONTROLLER pNew    = nullptr;
//    LPD3DXANIMATIONSET        pAddSet = nullptr;
//
//    for (auto i = std::next(m_characterSkinnedMeshResources.begin()); 
//              i != m_characterSkinnedMeshResources.end();
//            ++i)
//    {
//        LPD3DXANIMATIONCONTROLLER& pOld =
//            i->second->m_pSkinnedMesh->m_pAnimController;
//
//        UINT numDefaultAnim = pOld->GetMaxNumAnimationSets();
//
//        pOld->CloneAnimationController(
//            pOld->GetMaxNumAnimationOutputs(),
//            pOld->GetMaxNumAnimationSets() + m_numAddedAnim,
//            pOld->GetMaxNumTracks(),
//            pOld->GetMaxNumEvents(),
//            &pNew);
//
//        for (UINT i = numDefaultAnim; i < numDefaultAnim + m_numAddedAnim; ++i)
//        {
//            pC0AniCon->GetAnimationSet(i, &pAddSet);
//            pNew->RegisterAnimationSet(pAddSet);
//            pAddSet->Release();
//        }
//
//        pOld->Release();
//        pOld = pNew;
//    }
//
//    std::vector<ResourceContainer*> characters(Character::NUM_PLAYER);
//    for (auto c : m_characterSkinnedMeshResources)
//    {
//        characters[c.first] = c.second;
//    }
//    Resource()()->AddCharacters(characters);
//
//    m_isDoneCharacters = true;
//}

void SceneLoading::addEffectMeshs()
{
    ResourceManager* pRM = Resource()();

    for (auto pR : m_effectMeshResources)
        pRM->AddResource(pR.second);

    m_isDoneEffectMeshs = true;
}

bool SceneLoading::verifyTasks(tasks_t* OutTasks, resources_t* OutResources)
{
    assert(OutTasks && OutResources && 
        "SceneLoading::verifyTasks(), tasks or resources is null.");

    if (OutTasks->empty()) return true;

    std::future_status futureStatus;
    for (auto i = OutTasks->begin(); i != OutTasks->end();)
    {
        futureStatus = i->second.wait_until(std::chrono::system_clock::now());
        //futureStatus = i->second.wait_for(std::chrono::nanoseconds(1));
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
                ResourceContainer* pResourceContainer = i->second.get();
                OutResources->emplace(i->first, pResourceContainer);
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

void SceneLoading::addTask(const TAG_RES_STATIC tag, tasks_t* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = ResPathFileName::Get(tag);
    OutTasks->emplace_back(
        std::make_pair(
            OutTasks->size(),
            std::async(
                std::launch::async, 
                &ResourceAsync::OnLoadEffectMeshAsync, 
                keys.first, 
                keys.second)));

    ++m_numTotalTasks;
}

void SceneLoading::addTask(const TAG_RES_STATIC tag)
{
    addTask(tag, &m_effectMeshTasks);
}

void SceneLoading::addTask(const TAG_RES_ANIM_CHARACTER tag, tasks_t* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = ResPathFileName::Get(tag);
    OutTasks->emplace_back(
        std::make_pair(
            OutTasks->size(),
            std::async(
                std::launch::async, 
                &ResourceAsync::OnLoadSkinnedMeshAsync, 
                keys.first, 
                keys.second)));

    ++m_numTotalTasks;
}

void SceneLoading::addTask(const TAG_RES_ANIM_CHARACTER tag)
{
    addTask(tag, &m_characterAnimationTasks);
}
