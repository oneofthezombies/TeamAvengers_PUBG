#pragma once

// temp enums, must be deleted.
enum class TAG_RESOURCE_STATIC
{
    IDLE,
};

enum class TAG_RESOURCE_ANIM
{
    IDLE,
};

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

    size_t m_numTotalTasks;
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
        const TAG_RESOURCE_STATIC tag,
        std::deque<std::future<ResourceContainer*>>* OutTasks);
    void addTask(
        const TAG_RESOURCE_ANIM tag,
        std::deque<std::future<ResourceContainer*>>* OutTasks);

    pair<string, string> tempfunc(TAG_RESOURCE_STATIC tag);
    pair<string, string> tempfunc(TAG_RESOURCE_ANIM tag);

public:
             SceneLoading();
    virtual ~SceneLoading();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};