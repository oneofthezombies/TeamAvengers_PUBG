#pragma once
#include "TagResource.h"

class UIText;

class SceneLoading : public IScene
{
private:
    std::deque<std::future<ResourceContainer*>> m_effectMeshTasks;
    std::vector<ResourceContainer*>             m_effectMeshResources;

    std::deque<std::future<ResourceContainer*>> m_characterSkinnedMeshTasks;
    std::vector<ResourceContainer*> m_characterSkinnedMeshResources;

    std::deque<std::future<ResourceContainer*>> m_characterAnimationTasks;
    std::vector<ResourceContainer*>             m_characterAnimationResources;

    std::deque<std::future<ResourceContainer*>> m_skinnedMeshTasks;
    std::vector<ResourceContainer*>             m_skinnedMeshResources;

    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_finish;
    std::chrono::duration<float>          m_elapsed;

    std::size_t m_numTotalTasks;
    std::size_t m_numFinishedTasks;
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
    void loadAnimation();
    void addAnimationsToCharacter0();
    void copyAnimationsToOtherCharacters();
    void addEffectMeshs();

    bool verifyTasks(
        std::deque<std::future<ResourceContainer*>>* OutTasks, 
        std::vector<ResourceContainer*>* OutResources);

    void addTask(
        const TAG_RES_STATIC tag,
        std::deque<std::future<ResourceContainer*>>* OutTasks);
    void addTask(
        const TAG_RES_ANIM tag,
        std::deque<std::future<ResourceContainer*>>* OutTasks);

public:
             SceneLoading();
    virtual ~SceneLoading();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};