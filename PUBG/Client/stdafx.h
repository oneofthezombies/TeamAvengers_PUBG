#pragma once

#include "Common.h"
#pragma comment(lib, "Common.lib")

// FMOD
#include <fmod.hpp>
#include <fmod_errors.h>
#pragma comment(lib, "fmod_vc.lib")
// end FMOD

// debug
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#define D3DX_DEBUG
#define OOTZ_DEBUG
// end debug

// math and usefuls
#include "FVFStructure.h"
#include "ResourceStructure.h"
// end math and usefuls

// managers and interfaces for global access
#include "Application.h"     // for g_pApp
#include "Direct3D.h"        // for g_pD3D, g_pD3DDevice
#include "Debugger.h"        // for g_pDebugger, Debug
#include "UIManager.h"       // for g_pUIManager, g_pSprite
#include "FontManager.h"     // for g_pFontManager
#include "Input.h"           // for g_pInput
#include "SoundManager.h"    // for g_pSoundManager
#include "SceneManager.h"    // for g_pSceneManager,  g_pCurrentScene
#include "IScene.h"          // for IScene methods
#include "CameraManager.h"   // for g_pCameraManager, g_pCurrentCamera
#include "Camera.h"          // for Camera methods
#include "Client.h"          // for g_pCommunicator
#include "ResourceManager.h" // for g_pResourceManager
// end managers and interfaces

// global variables
extern HWND g_hWnd;
// end global variables
