#include "stdafx.h"
#include "SceneLoading.h"
#include "ResourceInfo.h"
#include "Character.h"
#include "UIText.h"

void SceneLoading::loadSync()
{
    std::pair<std::string, std::string> pathFilename;

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_STATIC::Ammo_5_56mm);
    m_effectMeshResources.emplace(0, ResourceAsync::OnLoadEffectMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_STATIC::Ammo_7_62mm);
    m_effectMeshResources.emplace(1, ResourceAsync::OnLoadEffectMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_STATIC::QBZ);
    m_effectMeshResources.emplace(2, ResourceAsync::OnLoadEffectMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_STATIC::Kar98k);
    m_effectMeshResources.emplace(3, ResourceAsync::OnLoadEffectMeshAsync(pathFilename.first, pathFilename.second));

    addEffectMeshs();

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_WEAPON::QBZ_Anim);
    m_skinnedMeshResources.emplace(0, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_WEAPON::Kar98k_Anim);
    m_skinnedMeshResources.emplace(1, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    addSkinnedMeshs();

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::ForTest);
    m_characterSkinnedMeshResources.emplace(0, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Rifle_Idling);
    m_characterAnimationResources.emplace(0, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Prone);
    m_characterAnimationResources.emplace(1, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Stand);
    m_characterAnimationResources.emplace(2, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Crouch);
    m_characterAnimationResources.emplace(3, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Rifle_OnBody);
    m_characterAnimationResources.emplace(4, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Rifle_Stand_PrimarySlot_OnHand);
    m_characterAnimationResources.emplace(5, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Rifle_Stand_SecondarySlot_OnHand);
    m_characterAnimationResources.emplace(6, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Weapon_Kar98k_Character);
    m_characterAnimationResources.emplace(7, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Weapon_QBZ_Character);
    m_characterAnimationResources.emplace(8, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    pathFilename = ResourceInfo::GetPathFileName(TAG_RES_ANIM_CHARACTER::Weapon_Kar98k_Reload_Test);
    m_characterAnimationResources.emplace(9, ResourceAsync::OnLoadSkinnedMeshAsync(pathFilename.first, pathFilename.second));

    addAnimationsToCharacter();
}

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

    //loadImage();
    //loadEffectMesh();
    //loadSkinnedMesh();

    m_pPercentageImage = 
        new UIText(
            Resource()()->GetFont(TAG_FONT::Default), 
            D3DXVECTOR2(1000.0f, 50.0f), 
            &m_percentage, 
            D3DCOLOR_XRGB(0, 255, 0), 
            nullptr);
    m_pPercentageImage->SetPosition(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
    m_pPercentageImage->SetDrawTextFormat(DT_LEFT | DT_VCENTER);

    loadSync();
}

void SceneLoading::OnUpdate()
{
    if (m_isFinished)
    {
        /**/
        addHeightmapResource();
        /**/
        
        Debug << "elapsed time : " << m_elapsed.count() << '\n';

        UI()()->Destroy(m_pPercentageImage);
        Scene()()->SetCurrentScene(TAG_SCENE::Play);
        //Scene()()->SetCurrentScene(TAG_SCENE::Login);
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

        if (!m_isDoneSkinnedMeshs)
        {
            if (verifyTasks(
                &m_skinnedMeshTasks,
                &m_skinnedMeshResources))
            {
                addSkinnedMeshs();
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
            m_isDoneCharacters &&
            m_isDoneSkinnedMeshs)
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

void SceneLoading::loadImage()
{
    ResourceContainer* pResourceContainer = new ResourceContainer;
    ResourceAsync::CreateTexture(
        "./Resource", "input_field.png", 
        pResourceContainer);
    Resource()()->AddResource(pResourceContainer);
}

void SceneLoading::loadEffectMesh()
{
    //addTask(TAG_RES_STATIC::SkySphere);
    //addTask(TAG_RES_STATIC::Church);

    //addTask(TAG_RES_STATIC::Head_Lv1);
    //addTask(TAG_RES_STATIC::Armor_Lv1);
    //addTask(TAG_RES_STATIC::Back_Lv1);

    addTask(TAG_RES_STATIC::Bandage);
    //addTask(TAG_RES_STATIC::FirstAidKit);
    //addTask(TAG_RES_STATIC::MedKit);

    addTask(TAG_RES_STATIC::Ammo_5_56mm);
    addTask(TAG_RES_STATIC::Ammo_7_62mm);

    addTask(TAG_RES_STATIC::QBZ);
    addTask(TAG_RES_STATIC::Kar98k);

    //addTask(TAG_RES_STATIC::RedDot);
    //addTask(TAG_RES_STATIC::Aimpoint2X);
    //addTask(TAG_RES_STATIC::ACOG);
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
    //addTask(TAG_RES_ANIM_CHARACTER::Lobby, &m_characterSkinnedMeshTasks);

    // character
    addTask(TAG_RES_ANIM_CHARACTER::ForTest, &m_characterSkinnedMeshTasks);

    // equipment
    //addTask(TAG_RES_EQUIPMENT::Head_Lv1_Anim, &m_equipmentSkinnedMeshTasks);

    loadCharacterAnimation();

    // weapon
    addTask(TAG_RES_ANIM_WEAPON::QBZ_Anim, &m_skinnedMeshTasks);
    addTask(TAG_RES_ANIM_WEAPON::Kar98k_Anim, &m_skinnedMeshTasks);
}

void SceneLoading::loadCharacterAnimation()
{
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_DoorOpen_And_Pickup);

    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Idling);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Attack);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Attack_FPP);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Pickup_FPP);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Jump);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Jump_FPP);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Landing);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion_Crouch);
    //addTask(TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion_Prone);

    addTask(TAG_RES_ANIM_CHARACTER::Rifle_Idling);
    addTask(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Prone);
    addTask(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Stand);
    addTask(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion_Crouch);

    addTask(TAG_RES_ANIM_CHARACTER::Rifle_OnBody);
    addTask(TAG_RES_ANIM_CHARACTER::Rifle_Stand_PrimarySlot_OnHand);
    addTask(TAG_RES_ANIM_CHARACTER::Rifle_Stand_SecondarySlot_OnHand);

    addTask(TAG_RES_ANIM_CHARACTER::Weapon_Kar98k_Character);
    addTask(TAG_RES_ANIM_CHARACTER::Weapon_QBZ_Character);

    //for test
    addTask(TAG_RES_ANIM_CHARACTER::Weapon_Kar98k_Reload_Test);
    //...
}

void SceneLoading::addAnimationsToCharacter()
{
    ResourceContainer* pCharacterResource = 
        m_characterSkinnedMeshResources.begin()->second;

    const auto pathFilename = ResourceInfo::GetCharacterPathFileName();
    
    pCharacterResource->m_pSkinnedMesh.first = 
        pathFilename.first + pathFilename.second;

    Resource()()->AddResource(pCharacterResource);

    SkinnedMesh* pCharacter = 
        Resource()()->GetSkinnedMesh(pathFilename.first, pathFilename.second); 

    bool res = pCharacter->Seperate("spine_02");
    assert(res && 
        "SceneLoading::addAnimationsToCharacter(), \
         SkinnedMesh::Seperate() failed.");

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

void SceneLoading::addEffectMeshs()
{
    ResourceManager* pRM = Resource()();

    for (auto pR : m_effectMeshResources)
        pRM->AddResource(pR.second);

    m_isDoneEffectMeshs = true;
}

void SceneLoading::addSkinnedMeshs()
{
    ResourceManager* pRM = Resource()();

    for (auto pR : m_skinnedMeshResources)
        pRM->AddResource(pR.second);

    m_isDoneSkinnedMeshs = true;
}

void SceneLoading::addHeightmapResource()
{
    ResourceContainer* pResourceContainer = new ResourceContainer;
    ResourceAsync::CreateEffect("./Resource/Heightmap/", "Heightmap.fx", pResourceContainer);
    ResourceAsync::CreateTexture("./Resource/Heightmap/", "Heightmap.jpg", pResourceContainer);
    Resource()()->AddResource(pResourceContainer);
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

    auto keys = ResourceInfo::GetPathFileName(tag);
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

    auto keys = ResourceInfo::GetPathFileName(tag);
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

void SceneLoading::addTask(const TAG_RES_EQUIPMENT tag, tasks_t* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = ResourceInfo::GetPathFileName(tag);
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

void SceneLoading::addTask(const TAG_RES_ANIM_WEAPON tag, tasks_t* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = ResourceInfo::GetPathFileName(tag);
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
