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
    CheckError(m_pSystem->set3DSettings(1.0f, 1.0f, 1.01f));
    //set3DSetting(진동수,거리에따른 진동 조절(게임내 1M를 설정해주면된다), 거리에따른 감쇠 비율(커질수록 소리 감소가 커진다))

    AddSound(TAG_SOUND::Kar98_NormalShoot, "Resource/Sound/kar98/Kar98_NormalShoot.mp3", FMOD_3D,false);
    AddSound(TAG_SOUND::Kar98_SilenceShoot, "Resource/Sound/kar98/Kar98_SilenceShoot.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Kar98_BoltMove0, "Resource/Sound/kar98/Kar98_BoltMove0.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Kar98_BoltMove1, "Resource/Sound/kar98/Kar98_BoltMove1.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Kar98_BoltMove2, "Resource/Sound/kar98/Kar98_BoltMove2.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Kar98_Reload0, "Resource/Sound/kar98/Kar98_Reload0.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Kar98_Reload1, "Resource/Sound/kar98/Kar98_Reload1.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Kar98_Reload2, "Resource/Sound/kar98/Kar98_Reload2.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Kar98_Reload3, "Resource/Sound/kar98/Kar98_Reload3.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Qbz_NormalShoot, "Resource/Sound/qbz/Qbz_NormalShoot.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Qbz_BoltMove76, "Resource/Sound/qbz/Qbz_BoltMove76.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Qbz_Reload, "Resource/Sound/qbz/Qbz_Reload.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Qbz_FakeShoot, "Resource/Sound/qbz/Qbz_FakeShoot.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Qbz_SilenceShoot, "Resource/Sound/qbz/Qbz_SilenceShoot.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Bullet_Miss, "Resource/Sound/Bullet/miss.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Bullet_HitTerrain, "Resource/Sound/Bullet/hit_terrain.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::FootStep, "Resource/Sound/footstep/footstep.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Hand_Punch, "Resource/Sound/Hand/Punch.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Medicine_Bandage, "Resource/Sound/medicine/Bandage.mp3", FMOD_3D, false);
    AddSound(TAG_SOUND::Medicine_Bandage, "Resource/Sound/medicine/Bandage.mp3", FMOD_3D, false);

    AddSound(TAG_SOUND::Background, "Resource/Sound/Background/battleground_lobby.mp3", FMOD_3D, false);

}

void SoundManager::Destroy()
{
    for (auto s : m_sounds)
        if (s.second) s.second->release();

    if (m_pSystem) m_pSystem->release();
}

void SoundManager::Update()
{
    const float dt = Time()()->GetDeltaTime();

    for (auto it = m_playInfo.begin(); it != m_playInfo.end();)
    {
        it->curtime -= dt;
        if (it->curtime > 0.0f)
        {
            ++it;
        }
        else
        {
            Play(it->TAG, it->pos, 1.0f, it->mode);
            it = m_playInfo.erase(it);
        }
    }

    if (m_pSystem) m_pSystem->update();
}

void SoundManager::AddSound(const TAG_SOUND tag, const string& path, const FMOD_MODE mode,const bool near)
{
	const auto search = m_sounds.find(tag);
	if (search != m_sounds.end()) return;

    CheckError(m_pSystem->createSound(path.c_str(), mode/*| FMOD_LOOP_NORMAL*/, nullptr, &m_sounds[tag]));
    //explain : 30까지는 본래의 소리를 냄 30이후부터 줄어들고 10000까지 Fmod::system에 저장한 감쇠량만큼 감소됨
    CheckError(m_sounds[tag]->set3DMinMaxDistance(300.0f, 10000.0f));
    // 총/폭발음 이외에는 디스턴스 조절 감소가 시작하는 시점을 아주 적은 시점부터 시작하여 소리를 작게만듬.
    if (static_cast<int>(tag) > 4)
    {
        CheckError(m_sounds[tag]->set3DMinMaxDistance(100.0f, 10000.0f));
    }
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

    search->second->setMode(FMOD_3D);
    CheckError(m_pSystem->playSound(search->second, nullptr, false, &m_channels[i]));
    m_channels[i]->set3DAttributes(&m_soundPos, nullptr);
    m_channels[i]->setVolume(m_volume);
    return static_cast<int>(i);
}

int SoundManager::Play(const TAG_SOUND tag, const D3DXVECTOR3& pos, const float vol, const FMOD_MODE& mode)
{
    const auto search = m_sounds.find(tag);
    if (search == m_sounds.end()) return -1;
    search->second->setMode(mode);

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

    //normalShoot 만 1이고 나머지는 0.8 
    float fVol = 1.0f;
    if (tag == TAG_SOUND::Background)
    {
    }
    else if (static_cast<int>(tag) > 2)
    {
        fVol = 0.7f;
    }

    SetPosition(pos);

    
    CheckError(m_pSystem->playSound(search->second, nullptr, false, &m_channels[i]));
    
   // m_channels[i]->setMode(FMOD_LOOP_NORMAL);
    m_channels[i]->set3DAttributes(&m_soundPos,nullptr);
    m_channels[i]->setVolume(fVol);
    return static_cast<int>(i);
}

int SoundManager::addPlay(const TAG_SOUND tag, const D3DXVECTOR3 & pos, float time, const FMOD_MODE & mode)
{
    PlayInfo PI = { tag, pos,mode, time };
    m_playInfo.push_back(PI);
    return 0;
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
    //만약을 위한 코딩


    //m_system->set3DListenerAttributes(0, &fVector, &vel, &m_forwardDir, &m_upDir);
    m_pSystem->set3DListenerAttributes(0, &m_listenerPos, nullptr, &m_forwardDir, &m_upDir);



    //Debug->AddText("Linsten");
    //Debug->AddText(D3DXVECTOR3(m_forwardDir.x, m_forwardDir.y, m_forwardDir.z));
    //Debug->EndLine();
}

void SoundManager::SetPosition(D3DXVECTOR3 pos)
{
    m_soundPos = { pos.x, pos.y , pos.z };
}
