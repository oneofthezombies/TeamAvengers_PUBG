#include "stdafx.h"
#include "SceneLoading.h"
#include "ResourceInfo.h"
#include "Character.h"
#include "UIText.h"
#include "UIImage.h"

void SceneLoading::Load()
{
    // set policy 
    // sync  -> on main thread
    // async -> multi threading
    setPolicy(Resource::Policy::ASYNC); //ASYNC

    // set play mode
    // alone       -> no network
    // with others -> login to network
    setPlayMode(Communication::PlayMode::WITH_OTHERS);
    //setPlayMode(Communication::PlayMode::ALONE);

    //// load effect meshs
    load(TAG_RES_STATIC::SkySphere);
    load(TAG_RES_STATIC::Ammo_5_56mm);
    load(TAG_RES_STATIC::Ammo_7_62mm);
    load(TAG_RES_STATIC::Armor_Lv1);
    load(TAG_RES_STATIC::Back_Lv1);
    load(TAG_RES_STATIC::Head_Lv1);
    load(TAG_RES_STATIC::QBZ);
    load(TAG_RES_STATIC::Kar98k);

    load(TAG_RES_STATIC::Bandage);
    load(TAG_RES_STATIC::FirstAidKit);
    load(TAG_RES_STATIC::MedKit);

    load(TAG_RES_STATIC::RedDot);
    load(TAG_RES_STATIC::Aimpoint2X);
    load(TAG_RES_STATIC::ACOG);

    load(TAG_RES_STATIC::Rock_1);

    load(TAG_RES_STATIC::WareHouse_A);

    // load skined meshs
    const int numQBZ = 4;
    for (int i = 0; i < numQBZ; ++i)
        load(TAG_RES_ANIM_WEAPON::QBZ_Anim);

    const int numKar98k = 4;
    for (int i = 0; i < numKar98k; ++i)
        load(TAG_RES_ANIM_WEAPON::Kar98k_Anim);

    // load character - Unarmed_Jump.X는 2개의 animation set을 가지고 있음
    for (int i = 0; i < GameInfo::NUM_PLAYERS; ++i)
        load(TAG_RES_ANIM_CHARACTER::Unarmed_Jump);

    // load equipment
    const int numArmor = 4;
    for (int i = 0; i < numArmor; ++i)
        load(TAG_RES_ANIM_EQUIPMENT::Armor_Lv1_Anim);

    const int numBack = 4;
    for (int i = 0; i < numBack; ++i)
        load(TAG_RES_ANIM_EQUIPMENT::Back_Lv1_Anim);

    const int numHead = 4;
    for (int i = 0; i < numHead; ++i)
        load(TAG_RES_ANIM_EQUIPMENT::Head_Lv1_Anim);

    // load animation
    addAnimation(TAG_RES_ANIM_CHARACTER::Unarmed_Locomotion);
    addAnimation(TAG_RES_ANIM_CHARACTER::Unarmed_Combined);

    addAnimation(TAG_RES_ANIM_CHARACTER::Rifle_Locomotion);
    addAnimation(TAG_RES_ANIM_CHARACTER::Rifle_Combined);

    // load animation FPP
    addAnimation(TAG_RES_ANIM_CHARACTER::Unarmed_Combined_FPP);
    addAnimation(TAG_RES_ANIM_CHARACTER::Rifle_Combined_FPP);
    addAnimation(TAG_RES_ANIM_CHARACTER::Weapon_Combined_FPP);
    addAnimation(TAG_RES_ANIM_CHARACTER::Healing_FPP);

    addAnimation(TAG_RES_ANIM_CHARACTER::DBNO);

    addAnimation(TAG_RES_ANIM_CHARACTER::Weapon_Kar98k_Character);
    addAnimation(TAG_RES_ANIM_CHARACTER::Weapon_QBZ_Character);

    /*
    Unarmed_Combined.X 는 하단의 X파일들을 합친 것
    - Lobby.X
    - Unarmed_Idle.X
    - Unarmed_Transition.X
    - Unarmed_Attack.X
    - Unarmed_DoorOpen_And_Pickup.X
    - Unarmed_Landing.X

    Rifle_Combined.X 는 하단의 X파일들을 합친 것
    - Rifle_Idling.X
    - Rifle_Transition.X
    - Rifle_DoorOpen_And_Pickup.X
    - Rifle_Landing.X
    - Rifle_Jump.X
    - Rifle_OnBody.X
    - Rifle_OnHand.X

    Unarmed_Combined_FPP.X 는 하단의 X파일들을 합친 것
    - Unarmed_Pickup_FPP.X
    - Unarmed_Locomotion_FPP.X
    - Unarmed_Jump_FPP.X
    - Unarmed_Attack_FPP.X

    Rifle_Combined_FPP.X 는 하단의 X파일들을 합친 것
    - Rifle_Pickup_FPP.X
    - Rifle_Locomotion_FPP.X

    Weapon_Combined_FPP.X 는 하단의 X파일들을 합친 것
    - Weapon_Kar98k_Character_FPP.X
    - Weapon_QBZ_Character_FPP.X
    */
}

void SceneLoading::OnInit()
{
    Resource()()->AddTexture("./Resource/", "input_field.png");
    Resource()()->AddTexture("./Resource/", "LoadingScreen.tga");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "black_1280_720_70.png");
    Resource()()->AddTexture("./Resource/", "dedenne.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Character/", "Female.png", D3DCOLOR_XRGB(188, 188, 188));
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "line.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Item/Equipment/", "icon_equipment_Armor_Lv1.png");
    Resource()()->AddTexture(TAG_RES_STATIC::Ammo_5_56mm, D3DCOLOR_XRGB(0, 0, 0));
    Resource()()->AddTexture(TAG_RES_STATIC::Ammo_7_62mm);
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "Equip_no.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "Equip_mouseover.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "Equip_yes.png");

    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "ItemSlot.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "ItemSlot_mouseover.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "Equip_click.png");

    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "WeaponBox_idle.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "WeaponBox_mouseover.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "WeaponBox_possible.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "Weapon_highlight.png");

    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "Attach_click.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "Attach_no.png");
    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "Attach_yes.png");

    Resource()()->AddTexture("./Resource/UI/Inventory/Basic/", "WeaponBoxNum.png");

    Resource()()->AddTexture("./Resource/UI/Inventory/Weapon/", "icon_weapon_Kar98k.png", D3DCOLOR_XRGB(0, 0, 0));
    Resource()()->AddTexture("./Resource/UI/Inventory/Weapon/", "icon_weapon_QBZ95.png", D3DCOLOR_XRGB(0, 0, 0));

    Resource()()->AddTexture("./Resource/UI/Inventory/Weapon/", "Kar98k.png", D3DCOLOR_XRGB(0, 0, 0));
    Resource()()->AddTexture("./Resource/UI/Inventory/Weapon/", "QBZ.png", D3DCOLOR_XRGB(0, 0, 0));

    //for InGameUI
    Resource()()->AddTexture("./Resource/UI/InGame/", "transparent_1280_720.png");

    Resource()()->AddTexture("./Resource/UI/InGame/", "compass_bg.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "compass_arrow_bg.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "compass.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "compass_arrow.png", D3DCOLOR_XRGB(0, 0, 0));

    Resource()()->AddTexture("./Resource/UI/InGame/", "equipment_bag01.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "equipment_helmet01.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "equipment_vest01.png");

    Resource()()->AddTexture("./Resource/UI/InGame/", "player_HP_BG_v3.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "player_HP_bar_v2.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "player_HP_bar_red.png");

    Resource()()->AddTexture("./Resource/UI/InGame/", "ammo_info.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "survival_text_bg.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "survival_num_bg.png");

    Resource()()->AddTexture("./Resource/UI/InGame/", "kill_num_up_bg.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "kill_text_up_bg.png");

    Resource()()->AddTexture("./Resource/UI/InGame/", "weapons_bg.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "weapons_gun_kar98k.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "weapons_gun_QBZ95.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "weapons_gun_kar98k_red.png");
    Resource()()->AddTexture("./Resource/UI/InGame/", "weapons_gun_QBZ95_red.png");

    Resource()()->AddTexture("./Resource/UI/InGame/", "map_bg.png");

    //for compass UI test
    Resource()()->AddTexture("./Resource/UI/InGame/", "compass_bg_transparent.png");

    m_pBackground =
        new UIImage(
            "./Resource/",
            "LoadingScreen.tga",
            Vector3::ZERO,
            nullptr,
            nullptr);
    UI()()->RegisterUIObject(m_pBackground);

    m_pPercentageImage =
        new UIText(
            Resource()()->GetFont(TAG_FONT::Default),
            D3DXVECTOR2(500.0f, 100.0f),
            &m_percentage,
            D3DCOLOR_XRGB(0, 255, 0),
            m_pBackground);
    m_pPercentageImage->SetPosition(D3DXVECTOR3(450.0f, 450.0f, 0.0f));
    m_pPercentageImage->SetDrawTextFormat(DT_LEFT | DT_VCENTER);

    m_start = std::chrono::system_clock::now();
    t = std::thread(std::bind(&SceneLoading::Load, this));

    m_channel = Sound()()->Play(TAG_SOUND::Background, Vector3::ZERO, 1.0f, FMOD_2D | FMOD_LOOP_NORMAL);
}

void SceneLoading::load(const TAG_RES_STATIC tag)
{
    std::pair<std::string, std::string> pathFilename = 
        ResourceInfo::GetPathFileName(tag);

    if (m_policy == Resource::Policy::SYNC)
    {
        m_tasksForSingleThread.emplace_back(
            std::make_tuple(
                &m_effectMeshResources, 
                std::bind(
                    &Resource::Async::OnLoadEffectMesh, 
                    std::placeholders::_1, 
                    std::placeholders::_2), 
                pathFilename.first, 
                pathFilename.second));

        ++m_numTotalTasks;
    }
    else if (m_policy == Resource::Policy::ASYNC)
    {
        addTask(tag, &m_effectMeshTasks);
    }
}

void SceneLoading::load(const TAG_RES_ANIM_WEAPON tag)
{
    std::pair<std::string, std::string> pathFilename =
        ResourceInfo::GetPathFileName(tag);

    if (m_policy == Resource::Policy::SYNC)
    {
        m_tasksForSingleThread.emplace_back(
            std::make_tuple(
                &m_skinnedMeshResources,
                std::bind(
                    &Resource::Async::OnLoadSkinnedMesh,
                    std::placeholders::_1,
                    std::placeholders::_2),
                pathFilename.first,
                pathFilename.second));

        ++m_numTotalTasks;
    }
    else if (m_policy == Resource::Policy::ASYNC)
    {
        addTask(tag, &m_skinnedMeshTasks);
    }
}

void SceneLoading::load(const TAG_RES_ANIM_EQUIPMENT tag)
{
    std::pair<std::string, std::string> pathFilename =
        ResourceInfo::GetPathFileName(tag);

    if (m_policy == Resource::Policy::SYNC)
    {
        m_tasksForSingleThread.emplace_back(
            std::make_tuple(
                &m_equipmentSkinnedMeshResources,
                std::bind(
                    &Resource::Async::OnLoadSkinnedMesh,
                    std::placeholders::_1,
                    std::placeholders::_2),
                pathFilename.first,
                pathFilename.second));

        ++m_numTotalTasks;
    }
    else if (m_policy == Resource::Policy::ASYNC)
    {
        addTask(tag, &m_equipmentSkinnedMeshTasks);
    }
}

void SceneLoading::load(const TAG_RES_ANIM_CHARACTER tag)
{
    std::pair<std::string, std::string> pathFilename =
        ResourceInfo::GetPathFileName(tag);

    if (m_policy == Resource::Policy::SYNC)
    {
        m_tasksForSingleThread.emplace_back(
            std::make_tuple(
                &m_characterSkinnedMeshResources,
                std::bind(
                    &Resource::Async::OnLoadSkinnedMesh,
                    std::placeholders::_1,
                    std::placeholders::_2),
                pathFilename.first,
                pathFilename.second));

        ++m_numTotalTasks;
    }
    else if (m_policy == Resource::Policy::ASYNC)
    {
        addTask(tag, &m_characterSkinnedMeshTasks);
    }
}

void SceneLoading::addAnimation(const TAG_RES_ANIM_CHARACTER tag)
{
    std::pair<std::string, std::string> pathFilename =
        ResourceInfo::GetPathFileName(tag);

    if (m_policy == Resource::Policy::SYNC)
    {
        m_tasksForSingleThread.emplace_back(
            std::make_tuple(
                &m_characterAnimationResources,
                std::bind(
                    &Resource::Async::OnLoadSkinnedMesh,
                    std::placeholders::_1,
                    std::placeholders::_2),
                pathFilename.first,
                pathFilename.second));

        ++m_numTotalTasks;
    }
    else if (m_policy == Resource::Policy::ASYNC)
    {
        addTask(tag, &m_characterAnimationTasks);
    }
}

void SceneLoading::setPolicy(const Resource::Policy policy)
{
    m_policy = policy;
}

void SceneLoading::setPlayMode(const Communication::PlayMode mode)
{
    Communication()()->SetPlayMode(mode);
}

bool SceneLoading::isFinished() const
{
    return 
        m_isDoneCharacters   && 
        m_isDoneEffectMeshs  && 
        m_isDoneSkinnedMeshs && 
        m_isDoneEquipments;
}

void SceneLoading::moveFinishedTasksForAsync(
    tasks_t* OutTasks, 
    resources_t* OutResources)
{
    assert(
        OutTasks && 
        OutResources &&
        "SceneLoading::moveFinishedTasks(), argument is null.");

    std::future_status futureStatus;
    for (auto it = OutTasks->begin(); it != OutTasks->end();)
    {
        //futureStatus = i->second.wait_until(std::chrono::system_clock::now());
        futureStatus = it->second.wait_for(std::chrono::milliseconds(100));
        switch (futureStatus)
        {
        case std::future_status::deferred:
            {
            }
            break;
        case std::future_status::timeout:
            {
                // waiting...
                ++it;
            }
            break;
        case std::future_status::ready:
            {
                Resource::XContainer* pXContainer = it->second.get();
                OutResources->emplace(it->first, pXContainer);
                it = OutTasks->erase(it);

                m_lastFinishedTaskName = pXContainer->m_filename;
                ++m_numFinishedTasks;
            }
            break;
        }
    }
}

void SceneLoading::addEffectMeshsForAsync()
{
    resources_t& rs = m_effectMeshResources;
    for (auto it = rs.begin(); it != rs.end();)
    {
        Resource()()->AddResource(it->second);
        it = rs.erase(it);
    }
}

void SceneLoading::addSkinnedMeshsForAsync()
{
    resources_t& rs = m_skinnedMeshResources;
    for (auto it = rs.begin(); it != rs.end();)
    {
        Resource()()->AddResource(it->second);
        it = rs.erase(it);
    }
}

void SceneLoading::addCharacterSkinnedMeshsForAsync()
{
    const auto pathFilename = ResourceInfo::GetCharacterPathFileName();

    resources_t& rs = m_characterSkinnedMeshResources;
    for (auto it = rs.begin(); it != rs.end();)
    {
        Resource::XContainer* pCharacterResource = it->second;
        pCharacterResource->m_pSkinnedMesh.first =
            pathFilename.first + pathFilename.second;

        Resource()()->AddResource(pCharacterResource);
        it = rs.erase(it);
    }
}

void SceneLoading::addCharacterAnimationsForAsync()
{
    const auto pathFilename = ResourceInfo::GetCharacterPathFileName();
    resources_t& rs = m_characterAnimationResources;

    for (std::size_t i = 0; i < GameInfo::NUM_PLAYERS; ++i)
    {
        SkinnedMesh* pCharacter =
            Resource()()->GetSkinnedMesh(
                pathFilename.first,
                pathFilename.second,
                i);

        LPD3DXANIMATIONCONTROLLER& pOld = pCharacter->m_pAnimController;

        LPD3DXANIMATIONCONTROLLER pAdd = nullptr;
        LPD3DXANIMATIONCONTROLLER pNew = nullptr;
        LPD3DXANIMATIONSET        pAddSet = nullptr;

        UINT numAddSet = 0;

        for (auto it = rs.begin(); it != rs.end(); ++it)
        {
            if (!it->second)
            {
                std::string text(std::to_string(it->first));
                MessageBoxA(nullptr, text.c_str(), nullptr, MB_OK);

                assert(
                    false &&
                    "SceneLoading::addAnimationsToCharacter(), XContainer is null.");
            }

            pAdd = it->second->m_pSkinnedMesh.second->m_pAnimController;

            pOld->CloneAnimationController(
                pOld->GetMaxNumAnimationOutputs(),
                pOld->GetMaxNumAnimationSets()
                + pAdd->GetMaxNumAnimationSets(),
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
        }
    }

    for (auto it = rs.begin(); it != rs.end();)
    {
        SAFE_DELETE(it->second);
        it = rs.erase(it);
    }
}

void SceneLoading::addEquipmentAnimationsForAsync()
{
    const auto characterPathFilename =
        ResourceInfo::GetCharacterPathFileName();

    SkinnedMesh* pCharacter =
        Resource()()->GetSkinnedMesh(
            characterPathFilename.first,
            characterPathFilename.second, 
            0);

    resources_t& rs = m_equipmentSkinnedMeshResources;

    for (auto it = rs.begin(); it != rs.end();)
    {
        Resource::XContainer* pResource = it->second;

        const std::string key = pResource->m_pSkinnedMesh.first;
        Resource()()->AddResource(pResource);

        std::size_t numSkinnedMesh = Resource()()->GetNumSkinnedMesh(key);

        SkinnedMesh* pSkinnedMesh =
            Resource()()->GetSkinnedMesh(key, numSkinnedMesh - 1);

        LPD3DXANIMATIONCONTROLLER& pOld = pSkinnedMesh->m_pAnimController;
        LPD3DXANIMATIONCONTROLLER  pAdd = pCharacter->m_pAnimController;
        LPD3DXANIMATIONCONTROLLER  pNew = nullptr;
        LPD3DXANIMATIONSET         pAddSet = nullptr;

        pOld->CloneAnimationController(
            pOld->GetMaxNumAnimationOutputs(),
            pOld->GetMaxNumAnimationSets() + pAdd->GetMaxNumAnimationSets(),
            pOld->GetMaxNumTracks(),
            pOld->GetMaxNumEvents(),
            &pNew);

        UINT numAddSet = pAdd->GetNumAnimationSets();
        for (UINT i = 0; i < numAddSet; ++i)
        {
            pAdd->GetAnimationSet(i, &pAddSet);
            pNew->RegisterAnimationSet(pAddSet);
            pAddSet->Release();
        }

        pOld->Release();
        pOld = pNew;

        bool res = pSkinnedMesh->Seperate("spine_02");
        assert(
            res &&
            "SceneLoading::addAnimationsToEquipment(), SkinnedMesh::Seperate() failed.");

        it = rs.erase(it);
    }
}

SceneLoading::SceneLoading()
    : IScene()
    , m_isDoneEquipments(false)
    , m_isFinished(false)
    , m_numAddedAnim(0)
    , m_numFinishedTasks(0)
    , m_numTotalTasks(0)
    , m_percentage("")
    , m_lastFinishedTaskName("")
    , m_dotDotDot(0)
    , m_policy(Resource::Policy::SYNC)
    , m_isDoneCharacterSkinnedMeshs(false)
    , m_isDoneCharacterAnimations(false)
    , m_isSperatedCharacters(false)
{
}

SceneLoading::~SceneLoading()
{
}

void SceneLoading::OnUpdate()
{
    if (t.joinable())
    {
        t.join();
    }

    if (isFinished())
    {
        m_finish = std::chrono::system_clock::now();
        m_elapsed = m_finish - m_start;

        addHeightmapResource();

        UI()()->Destroy(m_pBackground);

        if (Communication()()->GetPlayMode() == Communication::PlayMode::ALONE)
        {
            Scene()()->SetCurrentScene(TAG_SCENE::Play);
        }
        else if (Communication()()->GetPlayMode() == Communication::PlayMode::WITH_OTHERS)
        {
            Scene()()->SetCurrentScene(TAG_SCENE::Login);
        }
    }
    else
    {
        if (m_policy == Resource::Policy::ASYNC)
        {
            moveFinishedTasksForAsync(
                &m_effectMeshTasks, 
                &m_effectMeshResources);

            moveFinishedTasksForAsync(
                &m_skinnedMeshTasks, 
                &m_skinnedMeshResources);

            moveFinishedTasksForAsync(
                &m_characterSkinnedMeshTasks, 
                &m_characterSkinnedMeshResources);

            moveFinishedTasksForAsync(
                &m_characterAnimationTasks, 
                &m_characterAnimationResources);

            moveFinishedTasksForAsync(
                &m_equipmentSkinnedMeshTasks, 
                &m_equipmentSkinnedMeshResources);

            addEffectMeshsForAsync();
            addSkinnedMeshsForAsync();

            if (m_characterSkinnedMeshTasks.empty())
            {
                addCharacterSkinnedMeshsForAsync();

                if (m_characterSkinnedMeshResources.empty())
                {
                    addCharacterAnimationsForAsync();

                    if (m_characterAnimationTasks.empty() &&
                        m_characterAnimationResources.empty())
                    {
                        if (!m_isSperatedCharacters)
                        {
                            const auto pathFilename =
                                ResourceInfo::GetCharacterPathFileName();

                            for (std::size_t i = 0; i < GameInfo::NUM_PLAYERS; ++i)
                            {
                                SkinnedMesh* pCharacter =
                                    Resource()()->GetSkinnedMesh(
                                        pathFilename.first,
                                        pathFilename.second,
                                        i);

                                pCharacter->Seperate("spine_02");
                            }

                            m_isSperatedCharacters = true;
                        }

                        addEquipmentAnimationsForAsync();

                        if (m_equipmentSkinnedMeshTasks.empty() &&
                            m_equipmentSkinnedMeshResources.empty())
                        {
                            m_isDoneEffectMeshs = true;
                            m_isDoneSkinnedMeshs = true;
                            m_isDoneCharacters = true;
                            m_isDoneEquipments = true;

                        }
                    }
                }
            }
        }
        else if (m_policy == Resource::Policy::SYNC)
        {
            if (!m_tasksForSingleThread.empty())
            {
                auto begin = m_tasksForSingleThread.begin();
                resources_t* pResources = std::get<0>(*begin);
                auto&        task       = std::get<1>(*begin);
                std::string& path       = std::get<2>(*begin);
                std::string& xFilename  = std::get<3>(*begin);

                Resource::XContainer* pXContainer = task(path, xFilename);
                m_lastFinishedTaskName = pXContainer->m_filename;
                ++m_numFinishedTasks;

                pResources->emplace(pResources->size(), pXContainer);

                m_tasksForSingleThread.erase(begin);

                addEffectMeshsForAsync();
                addSkinnedMeshsForAsync();

                addCharacterSkinnedMeshsForAsync();
            }
            else
            {
                addCharacterAnimationsForAsync();

                if (!m_isSperatedCharacters)
                {
                    const auto pathFilename =
                        ResourceInfo::GetCharacterPathFileName();

                    for (std::size_t i = 0; i < GameInfo::NUM_PLAYERS; ++i)
                    {
                        SkinnedMesh* pCharacter =
                            Resource()()->GetSkinnedMesh(
                                pathFilename.first,
                                pathFilename.second,
                                i);

                        pCharacter->Seperate("spine_02");
                    }

                    m_isSperatedCharacters = true;
                }

                addEquipmentAnimationsForAsync();

                m_isDoneEffectMeshs = true;
                m_isDoneSkinnedMeshs = true;
                m_isDoneCharacters = true;
                m_isDoneEquipments = true;
            }
        }
    }

    Debug << "number of finished tasks : " << m_numFinishedTasks
        << " / number of total tasks : " << m_numTotalTasks << '\n';

    const float percentage = static_cast<float>(m_numFinishedTasks)
        / static_cast<float>(m_numTotalTasks)
        * 100.0f;

    ++m_dotDotDot;
    if (m_dotDotDot > 10)
        m_dotDotDot = 0;

    m_percentage = "Finished file : " + m_lastFinishedTaskName + "\n";
    m_percentage += "Percentage : " + std::to_string(percentage);
    for (std::size_t i = 0; i < m_dotDotDot; ++i)
        m_percentage += '.';
    m_percentage += '\n';
    for (int i = 0; i < static_cast<int>(percentage * 0.1f); ++i)
        m_percentage += "@";
    m_percentage += '\n';

    if (m_policy == Resource::Policy::SYNC)
    {
        m_percentage += "Loading on single thread...\n";
    }
    else if (m_policy == Resource::Policy::ASYNC)
    {
        m_percentage += "Loading on multi thread...\n";
    }
}

void SceneLoading::addAnimationsToCharacter()
{
    const auto pathFilename = ResourceInfo::GetCharacterPathFileName();

    for (std::size_t i = 0; i < GameInfo::NUM_PLAYERS; ++i)
    {
        SkinnedMesh* pCharacter =
            Resource()()->GetSkinnedMesh(
                pathFilename.first,
                pathFilename.second,
                i);

        LPD3DXANIMATIONCONTROLLER& pOld = pCharacter->m_pAnimController;

        LPD3DXANIMATIONCONTROLLER pAdd = nullptr;
        LPD3DXANIMATIONCONTROLLER pNew = nullptr;
        LPD3DXANIMATIONSET        pAddSet = nullptr;

        UINT numAddSet = 0;

        for (auto it = m_characterAnimationResources.begin(); it != m_characterAnimationResources.end();)
        {
            if (!it->second)
            {
                std::string text(std::to_string(it->first));
                MessageBoxA(nullptr, text.c_str(), nullptr, MB_OK);

                assert(
                    false &&
                    "SceneLoading::addAnimationsToCharacter(), \
                XContainer is null.");
            }

            pAdd = it->second->m_pSkinnedMesh.second->m_pAnimController;

            pOld->CloneAnimationController(
                pOld->GetMaxNumAnimationOutputs(),
                pOld->GetMaxNumAnimationSets()
                + pAdd->GetMaxNumAnimationSets(),
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
        }
    }

    for (auto it = m_characterAnimationResources.begin(); it != m_characterAnimationResources.end();)
    {
        SAFE_DELETE(it->second);
        it = m_characterAnimationResources.erase(it);
    }
}

void SceneLoading::addEffectMeshs()
{
    for (auto r : m_effectMeshResources)
    {
        Resource()()->AddResource(r.second);
    }

    m_effectMeshResources.clear();
}

void SceneLoading::addSkinnedMeshs()
{
    for (auto r : m_skinnedMeshResources)
    {
        Resource()()->AddResource(r.second);
    }

    m_skinnedMeshResources.clear();
}

void SceneLoading::addAnimationsToEquipment()
{
    const auto characterPathFilename = 
        ResourceInfo::GetCharacterPathFileName();
    
    SkinnedMesh* pCharacter =
        Resource()()->GetSkinnedMesh(
            characterPathFilename.first, 
            characterPathFilename.second, 0);

    for (auto it = m_equipmentSkinnedMeshResources.begin(); it != m_equipmentSkinnedMeshResources.end();)
    {
        Resource::XContainer* pResource = it->second;
        
        const std::string key = pResource->m_pSkinnedMesh.first;
        Resource()()->AddResource(pResource);

        std::size_t numSkinnedMesh = Resource()()->GetNumSkinnedMesh(key);

        SkinnedMesh* pSkinnedMesh = 
            Resource()()->GetSkinnedMesh(key, numSkinnedMesh - 1);

        LPD3DXANIMATIONCONTROLLER& pOld = pSkinnedMesh->m_pAnimController;
        LPD3DXANIMATIONCONTROLLER  pAdd = pCharacter->m_pAnimController;
        LPD3DXANIMATIONCONTROLLER  pNew = nullptr;
        LPD3DXANIMATIONSET         pAddSet = nullptr;

        pOld->CloneAnimationController(
            pOld->GetMaxNumAnimationOutputs(),
            pOld->GetMaxNumAnimationSets() + pAdd->GetMaxNumAnimationSets(),
            pOld->GetMaxNumTracks(),
            pOld->GetMaxNumEvents(),
            &pNew);

        UINT numAddSet = pAdd->GetNumAnimationSets();
        for (UINT i = 0; i < numAddSet; ++i)
        {
            pAdd->GetAnimationSet(i, &pAddSet);
            pNew->RegisterAnimationSet(pAddSet);
            pAddSet->Release();
        }

        pOld->Release();
        pOld = pNew;

        bool res = pSkinnedMesh->Seperate("spine_02");
        assert(
            res &&
            "SceneLoading::addAnimationsToEquipment(), \
             SkinnedMesh::Seperate() failed.");

        it = m_equipmentSkinnedMeshResources.erase(it);
    }
}

void SceneLoading::addHeightmapResource()
{
    Resource::XContainer* pResourceContainer = new Resource::XContainer;
    Resource::Async::CreateEffect(
        "./Resource/Heightmap/", 
        "Heightmap.fx", 
        pResourceContainer);
    Resource::Async::CreateTexture(
        "./Resource/Heightmap/", 
        "Heightmap.jpg", 
        pResourceContainer);
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
        //futureStatus = i->second.wait_until(std::chrono::system_clock::now());
        futureStatus = i->second.wait_for(std::chrono::milliseconds(100));
        switch (futureStatus)
        {
        case std::future_status::deferred:
            {
            }
            break;
        case std::future_status::timeout:
            {
                // waiting...
                ++i;
            }
            break;
        case std::future_status::ready:
            {
                Resource::XContainer* pXContainer = i->second.get();
                OutResources->emplace(i->first, pXContainer);
                i = OutTasks->erase(i);

                m_lastFinishedTaskName = pXContainer->m_filename;
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
                &Resource::Async::OnLoadEffectMesh, 
                keys.first, 
                keys.second)));

    ++m_numTotalTasks;
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
                &Resource::Async::OnLoadSkinnedMesh, 
                keys.first, 
                keys.second)));

    ++m_numTotalTasks;
}

void SceneLoading::addTask(const TAG_RES_ANIM_EQUIPMENT tag, tasks_t* OutTasks)
{
    assert(OutTasks && "SceneLoading::addTask(), tasks is null.");

    auto keys = ResourceInfo::GetPathFileName(tag);
    OutTasks->emplace_back(
        std::make_pair(
            OutTasks->size(),
            std::async(
                std::launch::async,
                &Resource::Async::OnLoadSkinnedMesh,
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
                &Resource::Async::OnLoadSkinnedMesh,
                keys.first,
                keys.second)));

    ++m_numTotalTasks;
}
