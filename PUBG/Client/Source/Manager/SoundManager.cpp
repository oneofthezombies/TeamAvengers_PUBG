#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
	: Singleton<SoundManager>()
	, m_pSystem(nullptr)
{
    for (auto& c : m_channels)
        c = nullptr;
}

SoundManager::~SoundManager()
{
}

void SoundManager::CheckError(const FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        char str[256];
        sprintf_s(str, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		assert(false && str);
    }
}

void SoundManager::Init()
{
	CheckError(FMOD::System_Create(&m_pSystem));
	CheckError(m_pSystem->init(MAX_CHANNEL_SIZE, FMOD_INIT_NORMAL, nullptr));
}

void SoundManager::Destroy()
{
    for (auto s : m_sounds)
        if (s.second) s.second->release();

    if (m_pSystem) m_pSystem->release();
}

void SoundManager::Update()
{
    if (m_pSystem) m_pSystem->update();
}

void SoundManager::AddSound(const TAG_SOUND tag, const string& path)
{
	const auto search = m_sounds.find(tag);
	if (search != m_sounds.end()) return;

    CheckError(m_pSystem->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_sounds[tag]));
}

int SoundManager::Play(const TAG_SOUND tag)
{
    const auto search = m_sounds.find(tag);
    if (search == m_sounds.end()) return -1;

    bool isPlaying = false;
    size_t i = 0u;
    for (; i < m_channels.size(); ++i)
    {
        if (!m_channels[i]) break;
        
        if (m_channels[i])
        {
            m_channels[i]->isPlaying(&isPlaying);
            if (!isPlaying) break;
        }
    }

    if (i == m_channels.size()) return -1;

    CheckError(m_pSystem->playSound(search->second, nullptr, false, &m_channels[i]));

    return i;
}

int SoundManager::Repeat(const TAG_SOUND tag, const int channelKey)
{
    if (channelKey == -1) return Play(tag);

    else if (channelKey < 0 || static_cast<size_t>(channelKey) >= m_channels.size()) return -1;

    if (!m_channels[channelKey]) return -1;

    bool isPlaying = false;
    m_channels[channelKey]->isPlaying(&isPlaying);
    if (isPlaying) return channelKey;

    return Play(tag);
}

int SoundManager::Pause(const int channelKey, const bool paused)
{
    if (channelKey < 0 || static_cast<size_t>(channelKey) >= m_channels.size()) return -1;

    if (!m_channels[channelKey]) return -1;

    m_channels[channelKey]->setPaused(paused);
    return channelKey;
}

int SoundManager::Stop(const int channelKey)
{
    if (channelKey < 0 || static_cast<size_t>(channelKey) >= m_channels.size()) return -1;

    if (!m_channels[channelKey]) return -1;

    m_channels[channelKey]->stop();
    return channelKey;
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
