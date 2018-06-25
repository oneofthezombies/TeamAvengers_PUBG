#pragma once
#include "Singleton.h"

class TimeManager : public Singleton<TimeManager>
{
private:
	struct FpsCalculator
	{
		float SumTime;
		int FrameCount;
		int Fps;

        FpsCalculator();

		void Update(const float dt);
	};

private:
    system_clock::time_point m_prevTime;
    float                    m_deltaTime;
    FpsCalculator			 m_fps;

             TimeManager();
	virtual ~TimeManager();

	void CalcDeltaTime();

public:
	void	Update();

	float	GetDeltaTime();
	int		GetFps();

	friend Singleton<TimeManager>;
};

struct Time
{
    TimeManager* operator()()
    {
        return TimeManager::GetInstance();
    }
};