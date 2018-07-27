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

    // true -> terrainFeature is not rendering
    DebugMgr ()()->Init(false);

	DeviceMgr()()->Init();
    Resource ()()->Init();
    Shader   ()()->Init();
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
    Shader       ()()->Destroy();
    DeviceMgr    ()()->Destroy();

    MemoryAllocator::CheckMemoryAllocators();
}

void Application::Update()
{
    DebugMgr     ()()->Clear();
    Time         ()()->Update();
    DebugMgr     ()()->Update();
    Communication()()->Print();
    Input        ()()->Update();
    Scene        ()()->Update();
    Sound        ()()->Update();
    Camera       ()()->Update();
    UI           ()()->Update();
}

void Application::Render()
{
    Device()()->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(50, 50, 50),
        1.0f,
        0);

    Device()()->BeginScene();

    if (!DebugMgr()()->IsHoonsComputer())
        Shader()()->CreateShadowMap();

    Device()()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device()()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    Device()()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    Device()()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    Scene       ()()->Render();
    BulletPool  ()()->Render();
    ParticlePool()()->Render();
    Light       ()()->Render();
    UI          ()()->Render();
    DebugMgr    ()()->Render();

    Device()()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

    Device()()->EndScene();
    Device()()->Present(nullptr, nullptr, nullptr, nullptr);
}

void Application::WndProc(HWND hWnd, UINT message, WPARAM wParam,
    LPARAM lParam)
{
    Input()()->WndProc(hWnd, message, wParam, lParam);
}
