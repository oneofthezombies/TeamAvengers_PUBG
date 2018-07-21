#pragma once

#include "Common.h"
#pragma comment(lib, "Common.lib")

// FMOD
#include <fmod.hpp>
#include <fmod_errors.h>
#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmod64_vc.lib")
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
#include "Ray.h"
#include "Collision.h"        // has Collision    ()()
#include "Light.h"
#include "Structure.h"
#include "Shader.h"
// end math and usefuls

// managers and interfaces for global access
// accessable method "ManagerName"::GetInstance() or ... below access instance.
#include "Application.h"      // has App          ()()
#include "DeviceManager.h"    // has Device       ()()
#include "Resource.h"         // has Resource     ()()
#include "UIManager.h"        // has UI           ()()
#include "SoundManager.h"     // has Sound        ()()
#include "TimeManager.h"      // has Time         ()()
#include "InputManager.h"     // has Input        ()()       
#include "SceneManager.h"     // has Scene        ()()
                              //     CurrentScene ()()
#include "CameraManager.h"    // has Camera       ()() 
                              //     CurrentCamera()()
#include "Client.h"           // has Communication()()
#include "Bullet.h"           // has BulletPool   ()()
#include "Particle.h"         // has ParticlePool ()()
#include "DebugManager.h"     // has DebugMgr     ()()
                              //     macro Debug

#include "IScene.h"
#include "ICamera.h"           
// end managers and interfaces

// global variables
extern HWND g_hWnd;
// end global variables
