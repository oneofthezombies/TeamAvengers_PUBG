#include "stdafx.h"
#include "TimeManager.h"


TimeManager::TimeManager()
{
    prevTime = system_clock::now();
	sumTime = 0;
	frameCnt = 0;
}


TimeManager::~TimeManager()
{
}

void TimeManager::Update()
{
    const auto currentTime = system_clock::now();
    duration<float> duration = currentTime - prevTime;
    deltaTime = duration.count();
    prevTime = currentTime;

	frameCnt++;
	sumTime += deltaTime;
	if (sumTime >= 0.2f)
	{
		fps = static_cast<int>(frameCnt / sumTime);
		frameCnt = 0;
		sumTime = 0;
	}

	Debug->AddText("FPS : ");
	Debug->AddText(fps);
	Debug->EndLine();
}
