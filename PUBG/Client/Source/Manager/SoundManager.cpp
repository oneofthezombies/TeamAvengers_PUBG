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
    //set3DSetting(������,�Ÿ������� ���� ����(���ӳ� 1M�� �������ָ�ȴ�), �Ÿ������� ���� ����(Ŀ������ �Ҹ� ���Ұ� Ŀ����))
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

void SoundManager::AddSound(const TAG_SOUND tag, const string& path, const FMOD_MODE mode)
{
	const auto search = m_sounds.find(tag);
	if (search != m_sounds.end()) return;

    CheckError(m_pSystem->createSound(path.c_str(), mode, nullptr, &m_sounds[tag]));
    //explain : 30������ ������ �Ҹ��� �� 30���ĺ��� �پ��� 10000���� Fmod::system�� ������ ���跮��ŭ ���ҵ�
    CheckError(m_sounds[tag]->set3DMinMaxDistance(30.0f, 10000.0f));
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

void SoundManager::Listen(const D3DXVECTOR3& listenerPos, const D3DXVECTOR3& listenerDir)
{
    //FMOD_VECTOR vel;
    //

    m_forwardDir = { listenerDir.x, listenerDir.y, listenerDir.z };
    m_listenerPos = { listenerPos.x, listenerPos.y, listenerPos.z };

    //vel.x = (listenerPos.x - m_lastpos.x) * (1000 / 100);
    //vel.y = (listenerPos.y - m_lastpos.y) * (1000 / 100);
    //vel.z = (listenerPos.z - m_lastpos.z) * (1000 / 100);
    //m_lastpos = { listenerDir.x, listenerDir.y, listenerDir.z };
    //������ ���� �ڵ�


    //m_system->set3DListenerAttributes(0, &fVector, &vel, &m_forwardDir, &m_upDir);
    m_pSystem->set3DListenerAttributes(0, &m_listenerPos, nullptr, &m_forwardDir, &m_upDir);



    //Debug->AddText("Linsten");
    //Debug->AddText(D3DXVECTOR3(m_forwardDir.x, m_forwardDir.y, m_forwardDir.z));
    //Debug->EndLine();
}
