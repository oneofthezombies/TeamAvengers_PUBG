#pragma once
#include "TagResource.h"

using tasks_t = std::deque<
    std::pair<
        std::size_t, 
        std::future<Resource::XContainer*>
    >
>;
using resources_t = std::map<std::size_t, Resource::XContainer*>;

enum class PlayMode
{
    // for local test
    ALONE,

    WITH_OTHERS
};

class UIText;
class UIImage;

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

    // equipment skinned mesh using character animation
    tasks_t     m_equipmentSkinnedMeshTasks;
    resources_t m_equipmentSkinnedMeshResources;

    // character animation
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
    UIImage*    m_pBackground;

    UINT m_numAddedAnim;
    bool m_isDoneCharacters;
    bool m_isDoneEffectMeshs;
    bool m_isDoneSkinnedMeshs;
    bool m_isFinished;

    Resource::Policy m_policy;
    PlayMode         m_playMode;

    std::thread t;

private:
    void addAnimationsToCharacter();
    void addEffectMeshs();
    void addSkinnedMeshs();

    void addHeightmapResource();

    bool verifyTasks(tasks_t* OutTasks, resources_t* OutResources);

    void addTask(const TAG_RES_STATIC         tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_ANIM_CHARACTER tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_EQUIPMENT      tag, tasks_t* OutTasks);
    void addTask(const TAG_RES_ANIM_WEAPON    tag, tasks_t* OutTasks);

    void load(const TAG_RES_STATIC         tag);
    void load(const TAG_RES_ANIM_WEAPON    tag);
    void load(const TAG_RES_EQUIPMENT      tag);
    void load(const TAG_RES_ANIM_CHARACTER tag);
    void addAnimation(const TAG_RES_ANIM_CHARACTER tag);

    void setPolicy(const Resource::Policy policy);
    void setPlayMode(const PlayMode mode);
    bool isFinished() const;

public:
             SceneLoading();
    virtual ~SceneLoading();

    virtual void OnInit() override;
    virtual void OnUpdate() override;

    void Load();
};