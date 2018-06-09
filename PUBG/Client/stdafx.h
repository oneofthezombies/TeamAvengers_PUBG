#pragma once

#include "CommonPreCompiled.h"

// FMOD
#include <fmod.hpp>
#include <fmod_errors.h>
#pragma comment(lib, "fmod_vc.lib")
// end FMOD

#pragma comment(lib, "Common.lib")

// managers and interfaces for global access
#include "Application.h"   // for g_pApp
#include "DeviceManager.h" // for g_pDeviceManager, g_pDevice
#include "KeyManager.h"    // for g_pKeyManager
#include "UIManager.h"     // for g_pUIManager, g_pSprite
#include "SoundManager.h"  // for g_pSoundManager
#include "SceneManager.h"  // for g_pSceneManager,  g_pCurrentScene
#include "CameraManager.h" // for g_pCameraManager, g_pCuurentCamera
// end managers and interfaces

// global variables
extern HWND g_hWnd;
// end global variables

// debug
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#define D3DX_DEBUG
// end debug
