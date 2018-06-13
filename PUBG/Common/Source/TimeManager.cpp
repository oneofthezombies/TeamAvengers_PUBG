#include "stdafx.h"
#include "TimeManager.h"

TimeManager::TimeManager()
	: Singleton<TimeManager>()
	, m_prevTime(system_clock::now())
	, m_deltaTime(0.0f)
	, m_fps()
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::CalcDeltaTime()
{
	const auto currentTime = system_clock::now();
	duration<float> elapsed = currentTime - m_prevTime;
	m_deltaTime = elapsed.count();
	m_prevTime = currentTime;
}

void TimeManager::Update()
{
	CalcDeltaTime();
	m_fps.Update(m_deltaTime);
}

float TimeManager::GetDeltaTime()
{
	return m_deltaTime;
}

int TimeManager::GetFPS()
{
	return m_fps.fps;
}

TimeManager::FPSCalculator::FPSCalculator()
	: sumTime(0.0f)
	, frameCount(0)
	, fps(0)
{
}

void TimeManager::FPSCalculator::Update(const float dt)
{
	++frameCount;
	sumTime += dt;
	if (sumTime >= 0.2f)
	{
		fps = static_cast<int>(frameCount / sumTime);
		frameCount = 0;
		sumTime = 0;
	}
}
