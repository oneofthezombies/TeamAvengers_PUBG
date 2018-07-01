#pragma once
#include "TagResource.h"

using tasks_t = std::deque<
    std::pair<
        std::size_t, 
        std::future<ResourceContainer*>
    >
>;
using resources_t = std::map<std::size_t, ResourceContainer*>;

class UIText;

class SceneLoading : public IScene
{
private:
    tasks_t     m_effectMeshTasks;
    resources_t m_effectMeshResources;

    tasks_t     m_skinnedMeshTasks;
    resources_t m_skinnedMeshResources;

    // character skinned mesh using character animation
    tasks_t     m_characterSkinnedMeshTasks;
    resources_t m_characterSkinnedMeshResources;

    // character animation
    tasks_t     m_characterAnimationTasks;
    resources_t m_characterAnimationResources;

    // equipment skinned mesh using character animation
    tasks_t     m_equipmentSkinnedMeshTasks;
    resources_t m_equipmentSkinnedMeshResources;

    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_finish;
    std::chrono::duration<float>          m_elapsed;

    std::size_t m_numTotalTasks;
    std::size_t m_numFinishedTasks;
    std::size_t m_dotDotDot;

    std::string m_lastFinishedTaskName;
    std::string m_percentage;
    UIText*     m_pPercentageImage;

    UINT m_numAddedAnim;
    bool m_isDoneCharacters;
    bool m_isDoneEffectMeshs;
    bool m_isDoneSkinnedMeshs;
    bool m_isFinished;

private:
    void loadEffectMesh();
    void loadSkinnedMesh();
    void loadCharacterAnimation();

    void addAnimationsToCharacter();
    void addEffectMeshs();
    void addSkinnedMeshs();

    void addHeightmap();

    bool verifyTasks(tasks_t* OutTasks, resources_t* OutResources);

    void addTask(const TAG_RES_STATIC tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_STATIC tag);
    void addTask(const TAG_RES_ANIM_CHARACTER tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_ANIM_CHARACTER tag);
    void addTask(const TAG_RES_EQUIPMENT tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_ANIM_WEAPON tag, tasks_t* OutTasks);

public:
             SceneLoading();
    virtual ~SceneLoading();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};