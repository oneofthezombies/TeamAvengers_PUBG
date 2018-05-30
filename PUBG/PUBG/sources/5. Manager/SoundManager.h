#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>

#pragma comment(lib, "fmod_vc.lib")

#include "SingletonBase.h"

#define g_pSoundManager SoundManager::GetInstance()

class SoundManager : public SingletonBase<SoundManager>
{
private:
    FMOD::System*  m_system;
    FMOD::Channel* m_channels[15];
    bool           m_isCheck[15];
    float          m_volume;
    unordered_map<SOUND_TAG, FMOD::Sound*> m_sounds;

public:
    friend SingletonBase<SoundManager>;

private:
    SoundManager();
    virtual ~SoundManager();

    void ERRCHECK(FMOD_RESULT result);

public:
    void Init();
    void Destroy();
    void RegisterSound(const SOUND_TAG& sound, string path);

    void Play(int channelKey, const SOUND_TAG& sound);
    void RepeatPlay(int channelKey, const SOUND_TAG& sound);
    void Pause(int channelKey, bool paused);
    void Update();
    void Stop(int channelKey);

    void VolumeUp(const float volume);
    void VolumeDown(const float volume);

    void SetVolume(const float volume);
    float GetVolume() const;
};