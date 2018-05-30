#include "stdafx.h"
#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
    g_pDeviceManager->Init();
    g_pKeyManager->Init();
    g_pUIManager->Init();
    g_pSoundManager->Init();
    g_pSceneManager->Init();
    g_pCameraManager->Init();
    g_pCollisionManager->Init();
}

void Application::Destroy()
{
    g_pCollisionManager->Destroy();
    g_pUIManager->Destroy();
    g_pSoundManager->Destroy();
	g_pSceneManager->Destroy();
    g_pCameraManager->Destroy();
	g_pObjectManager->Destroy();
	g_pTextureManager->Destroy();
	g_pFontManager->Destroy();
	Debug->Destroy();
	g_pKeyManager->Destroy();
	g_pDeviceManager->Destroy();
}

void Application::Update()
{
	Debug->InitText();
	g_pTimeManager->Update();
    g_pKeyManager->Update();
    g_pSoundManager->Update();
	g_pSceneManager->Update();
    g_pCameraManager->Update();
    g_pCollisionManager->Update();
    g_pUIManager->Update();
}

void Application::Render()
{
	g_pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);

	g_pDevice->BeginScene();

	g_pSceneManager->Render();
    g_pCollisionManager->Render();
    g_pUIManager->Render();
	Debug->Print();

	if (GetAsyncKeyState('M') & 0x0001)
		Debug->ShowMessageBox();

	g_pDevice->EndScene();
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}

void Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_pSceneManager->WndProc(hWnd, message, wParam, lParam);
	//g_pCameraManager->WndProc(hWnd, message, wParam, lParam);
    //g_pCamera->WndProc(hWnd, message, wParam, lParam);
}
