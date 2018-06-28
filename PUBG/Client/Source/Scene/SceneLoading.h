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

    tasks_t     m_characterSkinnedMeshTasks;
    resources_t m_characterSkinnedMeshResources;

    tasks_t     m_characterAnimationTasks;
    resources_t m_characterAnimationResources;

    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_finish;
    std::chrono::duration<float>          m_elapsed;

    std::size_t m_numTotalTasks;
    std::size_t m_numFinishedTasks;
    std::size_t m_dotDotDot;

    std::string m_lastFinishedTaskName;
    std::string m_percentage;
    UIText*     m_pPercentageImage;

    UINT   m_numAddedAnim;
    bool   m_isDoneCharacters;
    bool   m_isDoneEffectMeshs;
    bool   m_isFinished;

private:
    void loadEffectMesh();
    void loadSkinnedMesh();
    void loadCharacterAnimation();
    void addAnimationsToCharacter();
    //void copyAnimationsToOtherCharacters();
    void addEffectMeshs();

    bool verifyTasks(tasks_t* OutTasks, resources_t* OutResources);

    void addTask(const TAG_RES_STATIC tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_STATIC tag);
    void addTask(const TAG_RES_ANIM_CHARACTER tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_ANIM_CHARACTER tag);

public:
             SceneLoading();
    virtual ~SceneLoading();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};