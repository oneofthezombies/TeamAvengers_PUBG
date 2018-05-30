#pragma once

#define g_pApp Application::GetInstance()

class Application
{
private:
	Application();
	~Application();

public:
	static Application* GetInstance()
	{
		static Application instance;
		return &instance;
	}

	void Init();
	void Destroy();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

