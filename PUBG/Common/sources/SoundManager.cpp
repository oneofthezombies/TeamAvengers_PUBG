#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
	: Singleton<SoundManager>()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        char str[256];
        sprintf_s(str, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        //MessageBox(NULL, str, TEXT("TEST"), MB_OK);
        cout << str << endl;
    }
}

void SoundManager::Init()
{
    ERRCHECK(FMOD::System_Create(&m_system));
    ERRCHECK(m_system->init(12, FMOD_INIT_NORMAL, 0));

    /* 사운드 파일은 여기서 등록 */
    //registerSound(SOUND_TAG::Logo, "assets/sound/logo.mp3");
    //RegisterSound(SOUND_TAG::Lobby, "resources/sounds/battleground_lobby.mp3");
    //RegisterSound(SOUND_TAG::OneShot, "resources/sounds/kar98k_shoot.mp3");
    //RegisterSound(SOUND_TAG::Burst, "resources/sounds/burst.mp3");
    //RegisterSound(SOUND_TAG::Reload, "resources/sounds/kar98k_reloading_one_at_the_time.mp3");
    //RegisterSound(SOUND_TAG::ButtonClick, "resources/sounds/button_click.mp3");
    //RegisterSound(SOUND_TAG::Background, "resources/sounds/background.mp3");
    //RegisterSound(SOUND_TAG::Footstep, "resources/sounds/footstep.mp3");
    //RegisterSound(SOUND_TAG::DieMale, "resources/sounds/die_male2.mp3");
    //RegisterSound(SOUND_TAG::DieFemale, "resources/sounds/die_female.mp3");
    //RegisterSound(SOUND_TAG::PutIn, "resources/sounds/put_in.mp3");
    //RegisterSound(SOUND_TAG::InHand, "resources/sounds/in_hand.mp3");
    //RegisterSound(SOUND_TAG::OutHand, "resources/sounds/out_hand.mp3");
}

void SoundManager::Destroy()
{
    for (auto sound : m_sounds)
        if (sound.second)
            sound.second->release();

    if (m_system)
        m_system->release();
}

void SoundManager::RegisterSound(const SOUND_TAG & sound, string path)
{
    auto key = sound;
    auto find = m_sounds.find(key);
    if (find == m_sounds.end())
        ERRCHECK(m_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &m_sounds[key]));
    else
        assert(false && "SoundManager::RegisterSound()");
}

void SoundManager::Play(int channelKey, const SOUND_TAG & sound)
{
    auto find = m_sounds.find(sound);
    if (find != m_sounds.end())
        ERRCHECK(m_system->playSound(find->second, NULL, false, &m_channels[channelKey]));
    else
        assert(false && "Sound::Play()");
}

void SoundManager::RepeatPlay(int channelKey, const SOUND_TAG & sound)
{
    bool isPlay;
    m_channels[channelKey]->isPlaying(&isPlay);
    if (!isPlay)
    {
        Play(channelKey, sound);
    }
}

void SoundManager::Pause(int channelKey, bool paused)
{
    m_channels[channelKey]->setPaused(paused);
}

void SoundManager::Update()
{
    m_system->update();
}

void SoundManager::Stop(int channelKey)
{
    m_channels[channelKey]->stop();
}

void SoundManager::VolumeUp(const float volume)
{
    SetVolume(m_volume + volume);
}

void SoundManager::VolumeDown(const float volume)
{
    SetVolume(m_volume - volume);
}

void SoundManager::SetVolume(const float volume)
{
    if (volume < 0.0f)
        m_volume = 0.0f;
    else if (volume > 1.0f)
        m_volume = 1.0f;
    else
        m_volume = volume;
}

float SoundManager::GetVolume() const
{
    return m_volume;
}
