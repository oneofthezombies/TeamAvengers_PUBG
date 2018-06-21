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
	g_pD3D->Init();
    g_pUIManager->Init();
    g_pInput->Init();
    //g_pSoundManager->Init();
    g_pCameraManager->Init();
    g_pCollisionManager->Init();
    g_pSceneManager->Init();
}

void Application::Destroy()
{
    g_pCommunicator->Destroy();
    g_pCameraManager->Destroy();
	g_pSceneManager->Destroy();
    g_pUIManager->Destroy();
    g_pCollisionManager->Destroy();
	g_pObjectManager->Destroy();
    g_pInput->Destroy();
    //g_pSoundManager->Destroy();
    g_pResourceManager->Destroy();
	g_pFontManager->Destroy();
	g_pDebugger->Destroy();
	g_pD3D->Destroy();
}

void Application::Update()
{
	g_pDebugger->Clear();
	g_pTime->Update();
    g_pInput->Update();
    g_pSceneManager->Update();
    //g_pSoundManager->Update();
    g_pCameraManager->Update();
    g_pCollisionManager->Update();
    g_pUIManager->Update();

	Debug << "FPS : " << g_pTime->GetFPS() << '\n';
    g_pCommunicator->Logging();
}

void Application::Render()
{
	g_pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);

	g_pDevice->BeginScene();

	g_pSceneManager->Render();
    g_pUIManager->Render();
	g_pDebugger->Print();

	g_pDevice->EndScene();
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}

void Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    g_pInput->WndProc(hWnd, message, wParam, lParam);
}
