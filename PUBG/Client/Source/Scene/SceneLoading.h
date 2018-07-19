#pragma once
#include "TagResource.h"

using tasks_t = std::deque<
    std::pair<
        std::size_t, 
        std::future<Resource::XContainer*>
    >
>;
using resources_t = std::map<std::size_t, Resource::XContainer*>;

class UIText;
class UIImage;

class SceneLoading : public IScene
{
private:
    tasks_t     m_textureTasks;
    resources_t m_textureResources;

    tasks_t     m_effectMeshTasks;
    resources_t m_effectMeshResources;

    tasks_t     m_skinnedMeshTasks;
    resources_t m_skinnedMeshResources;

    // character skinned mesh using character animation
    tasks_t     m_characterSkinnedMeshTasks;
    resources_t m_characterSkinnedMeshResources;

    // equipment skinned mesh using character animation
    tasks_t     m_equipmentSkinnedMeshTasks;
    resources_t m_equipmentSkinnedMeshResources;

    // character animation
    tasks_t     m_characterAnimationTasks;
    resources_t m_characterAnimationResources;

    std::deque<
        std::tuple<
            resources_t*, 
            std::function<Resource::XContainer*(const std::string, const std::string)>,
            std::string,
            std::string
        >
    > m_tasksForSingleThread;

    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_finish;
    std::chrono::duration<float>          m_elapsed;

    std::size_t m_numTotalTasks;
    std::size_t m_numFinishedTasks;
    std::size_t m_dotDotDot;

    std::string m_lastFinishedTaskName;
    std::string m_percentage;
    std::string m_loadingDesc;
    std::string m_percentageDot;

    UIText*     m_pLoadingDescText;
    UIText*     m_pPercentageText;
    UIText*     m_pPercentageDot;

    UIImage*    m_pBackground;

    UINT m_numAddedAnim;
    bool m_isDoneCharacters;
    bool m_isDoneEffectMeshs;
    bool m_isDoneSkinnedMeshs;
    bool m_isDoneEquipments;
    bool m_isFinished;
    bool m_isDoneCharacterSkinnedMeshs;
    bool m_isDoneCharacterAnimations;
    bool m_isSperatedCharacters;
    bool m_isDoneTextures;

    Resource::Policy m_policy;

    int m_channel;

    std::thread t;

private:
    void addAnimationsToCharacter();
    void addEffectMeshs();
    void addSkinnedMeshs();
    void addAnimationsToEquipment();

    void addHeightmapResource();

    bool verifyTasks(tasks_t* OutTasks, resources_t* OutResources);

    void addTask(const TAG_RES_STATIC         tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_ANIM_CHARACTER tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_ANIM_EQUIPMENT tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_ANIM_WEAPON    tag, tasks_t* OutTasks);

    void load(
        const std::string path,
        const std::string xFilename,
        const D3DCOLOR colorKey = 0);

    void load(const TAG_RES_STATIC         tag);
    void load(const TAG_RES_ANIM_WEAPON    tag);
    void load(const TAG_RES_ANIM_EQUIPMENT tag);
    void load(const TAG_RES_ANIM_CHARACTER tag);
    void addAnimation(const TAG_RES_ANIM_CHARACTER tag);

    void setPolicy(const Resource::Policy policy);
    void setPlayMode(const Communication::PlayMode mode);
    bool isFinished() const;

    // for async
    void moveFinishedTasksForAsync(
        tasks_t* OutTasks, 
        resources_t* OutResources);

    void addTexturesForAsync();
    void addEffectMeshsForAsync();
    void addSkinnedMeshsForAsync();
    void addCharacterSkinnedMeshsForAsync();
    void addCharacterAnimationsForAsync();
    void addEquipmentAnimationsForAsync();

public:
             SceneLoading();
    virtual ~SceneLoading();

    virtual void OnInit() override;
    virtual void OnUpdate() override;

    void Load();
};