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
#include "Structure.h"
// end math and usefuls

// managers and interfaces for global access
// accessable method "ManagerName"::GetInstance() or ... below access instance.
#include "Application.h"      // has App          ()()
#include "DeviceManager.h"    // has Device       ()()
#include "ResourceManager.h"  // has Resource     ()()
#include "UIManager.h"        // has UI           ()()
#include "SoundManager.h"     // has Sound        ()()
#include "TimeManager.h"      // has Time         ()()
#include "CollisionManager.h" // has Collision    ()()
#include "InputManager.h"     // has Input        ()()       
#include "SceneManager.h"     // has Scene        ()()
                              //     CurrentScene ()()
#include "CameraManager.h"    // has Camera       ()() 
                              //     CurrentCamera()()
#include "Client.h"           // has Communication()()
#include "Bullet.h"           // has BulletPool   ()()
#include "DebugManager.h"     // has DebugMgr     ()()
                              //     macro Debug

#include "IScene.h"
#include "ICamera.h"           
// end managers and interfaces

// global variables
extern HWND g_hWnd;
// end global variables
