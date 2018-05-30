#pragma once

#define g_pTimeManager TimeManager::GetInstance()

class TimeManager
{
private:
    system_clock::time_point prevTime;
    float                    deltaTime;
	float	                 sumTime;
	int		                 frameCnt;
	int		                 fps;

	TimeManager();
	~TimeManager();

public:
	//ΩÃ±€≈Ê
	static TimeManager* GetInstance()
	{
		static TimeManager instance;
		return &instance;
	}

	void	Update();
    float	GetDeltaTime() { return deltaTime; }
	int		GetFPS() { return fps; }
};

