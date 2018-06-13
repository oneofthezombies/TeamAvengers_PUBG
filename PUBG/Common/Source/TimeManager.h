#pragma once
#include "Singleton.h"

#define g_pTime TimeManager::GetInstance()

class TimeManager : public Singleton<TimeManager>
{
public:
	struct FPSCalculator
	{
		float sumTime;
		int frameCount;
		int fps;

		FPSCalculator();

		void Update(const float dt);
	};

private:
    system_clock::time_point m_prevTime;
    float                    m_deltaTime;
	FPSCalculator			 m_fps;

    TimeManager();
	~TimeManager();

	void CalcDeltaTime();

public:
	void	Update();

	float	GetDeltaTime();
	int		GetFPS();

	friend Singleton<TimeManager>;
};

