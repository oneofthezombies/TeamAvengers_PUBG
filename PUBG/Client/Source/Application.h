#pragma once
#include "Singleton.h"

#define g_pApp Application::GetInstance()

class Application : public Singleton<Application>
{
private:
	Application();
	virtual ~Application();

public:
	void Init();
	void Destroy();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	friend Singleton<Application>;
};

