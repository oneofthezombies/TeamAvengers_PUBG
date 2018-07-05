#include "stdafx.h"
#include "Application.h"

Application::Application()
	: Singleton<Application>()
{
}

Application::~Application()
{
}

void Application::Init()
{
    srand(GetTickCount());
	DeviceMgr()()->Init();
    Resource ()()->Init();
    UI       ()()->Init();
    Input    ()()->Init();
    Sound    ()()->Init();
    Camera   ()()->Init();
    Scene    ()()->Init();
}

void Application::Destroy()
{
    Communication()()->Destroy();
    Camera       ()()->Destroy();
	Scene        ()()->Destroy();
    UI           ()()->Destroy();
    BulletPool   ()()->Destroy();
    Input        ()()->Destroy();
    Sound        ()()->Destroy();
    Resource     ()()->Destroy();
    DebugMgr     ()()->Destroy();
    DeviceMgr    ()()->Destroy();

    MemoryAllocator::CheckMemoryAllocators();
}

void Application::Update()
{
    DebugMgr     ()()->Clear();
    Time         ()()->Update();
    Communication()()->Print();
    BulletPool   ()()->PrintNumBullet();
    Input        ()()->Update();
    Device       ()()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                             D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);
    Device       ()()->BeginScene();
    Scene        ()()->Update();
    Sound        ()()->Update();
    Camera       ()()->Update();
    //Collision    ()()->Update();
    UI           ()()->Update();

    Scene        ()()->Render();

    UI           ()()->Render();
    DebugMgr     ()()->Render();
    Device       ()()->EndScene();
    Device       ()()->Present(nullptr, nullptr, nullptr, nullptr);
}

void Application::WndProc(HWND hWnd, UINT message, WPARAM wParam,
    LPARAM lParam)
{
    Input()()->WndProc(hWnd, message, wParam, lParam);
}
