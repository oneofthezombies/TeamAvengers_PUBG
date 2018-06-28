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

    Debug << "FPS : " << Time()()->GetFps() << '\n';
}

float TimeManager::GetDeltaTime()
{
	return m_deltaTime;
}

int TimeManager::GetFps()
{
	return m_fps.Fps;
}

TimeManager::FpsCalculator::FpsCalculator()
	: SumTime(0.0f)
	, FrameCount(0)
	, Fps(0)
{
}

void TimeManager::FpsCalculator::Update(const float dt)
{
	++FrameCount;
	SumTime += dt;
	if (SumTime >= 0.2f)
	{
		Fps = static_cast<int>(FrameCount / SumTime);
		FrameCount = 0;
		SumTime = 0;
	}
}
