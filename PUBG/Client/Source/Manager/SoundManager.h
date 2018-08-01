#pragma once
#include "Singleton.h"
#include "TagClientOnly.h"

#define g_pSound Sound()()

struct PlayInfo
{
    TAG_SOUND TAG;
    D3DXVECTOR3 pos;
    FMOD_MODE mode;
    float curtime;
};

class SoundManager : public Singleton<SoundManager>
{
private:
    static const int MAX_CHANNEL_SIZE = 64;

    FMOD::System*                           m_pSystem;
    array<FMOD::Channel*, MAX_CHANNEL_SIZE> m_channels;
    unordered_map<TAG_SOUND, FMOD::Sound*>  m_sounds;
    float                                   m_volume;


    FMOD_VECTOR     m_soundPos;
    FMOD_VECTOR     m_forwardDir;
    FMOD_VECTOR     m_upDir = { 0,1,0 };
    FMOD_VECTOR     m_listenerPos;

    std::deque<PlayInfo> m_playInfo;
    std::deque<PlayInfo> m_playInfoQueue;

private:
    SoundManager();
    virtual ~SoundManager();

    void CheckError(const FMOD_RESULT result);

public:
    void Init();
    void Destroy();
    void Update();

    //parameter(사운드 태그, 파일 패스, 3d/2d 선택);
    void AddSound(const TAG_SOUND tag, const string& path, const FMOD_MODE mode,const bool near);

    int Play(const TAG_SOUND tag);
    int Play(const TAG_SOUND tag, const D3DXVECTOR3& pos, const float vol, const FMOD_MODE& mode);
    int addPlay(const TAG_SOUND tag, const D3DXVECTOR3& pos, float time, const FMOD_MODE& mode);
    int Repeat(const TAG_SOUND tag, const int channelKey);
    int Pause(const int channelKey, const bool paused);
    int Stop(const int channelKey);

    void VolumeUp(const float volume);
    void VolumeDown(const float volume);

    void SetVolume(const float volume);
    float GetVolume() const;

    void Listen(const D3DXVECTOR3 &listenerPos, const D3DXVECTOR3 &listenerDir);
    void SetPosition(D3DXVECTOR3 pos);

	friend Singleton<SoundManager>;
};

struct Sound
{
    SoundManager* operator()()
    {
        return SoundManager::GetInstance();
    }
};