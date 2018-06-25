#pragma once

class SceneLoading : public IScene
{
private:
    std::future<ResourceContainer*>       m_future;
    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_finish;
    std::chrono::duration<float>          m_elapsed;
    bool m_isFinished;

public:
             SceneLoading();
    virtual ~SceneLoading();

    virtual void OnInit() override;
    virtual void OnUpdate() override;
};