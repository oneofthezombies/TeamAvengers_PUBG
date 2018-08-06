#pragma once
#include "Singleton.h"

#define g_pApp App()()

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

struct App
{
    Application* operator()()
    {
        return Application::GetInstance();
    }
};