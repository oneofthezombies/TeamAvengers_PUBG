#pragma once

// windows headers
// exclude rarely used content from the Windows header.
#define WIN32_LEAN_AND_MEAN
// for std::numeric_limits<T>::max() and min(), windows MIN and MAX macro is ambiguous with this methods
#define NOMINMAX    
#include <windows.h>
// end windows headers

// c runtime headers
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <atlstr.h>
#include <cassert>
// end c runtime headers

// STLs
#include <fstream>
using std::fstream;

#include <chrono>
using std::chrono::system_clock;
using std::chrono::duration;

#include <iterator>
using std::advance;
using std::next;
using std::prev;

#include <iostream>
using std::cout;
using std::endl;

#include <limits>
using std::numeric_limits;

#include <utility>
using std::make_pair;
using std::pair;

#include <type_traits>
using std::is_base_of_v;
using std::enable_if_t;

#include <functional>
using std::hash;

#include <typeindex>
using std::type_index;

#include <ostream>
using std::ostream;

// STL data structures
#include <unordered_map>
using std::unordered_multimap;
using std::unordered_map;

#include <unordered_set>
using std::unordered_multiset;
using std::unordered_set;

#include <string>
using std::to_string;
using std::string;

#include <bitset>
using std::bitset;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

#include <set>
using std::set;
// end STL data structures
// end STLs

// directx9
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
// end directx9

// essential utilities
#include "GlobalDefinition.h"
#include "D3DUtility.h"
// end essential utilities

// manager classes and some base classes
// manager class has a function (or variable) that can be accessed globally.
#include "Debug.h"              // for Debug
#include "Application.h"        // for g_pApp
#include "DeviceManager.h"      // for g_pDeviceManager and g_pDevice
#include "ObjectManager.h"      // for g_pObjectManager
#include "TextureManager.h"     // for g_pTextureManager
#include "TimeManager.h"        // for g_pTimeManager
#include "SceneManager.h"       // for g_pSceneManager
#include "IScene.h"             // for g_pCurrentScene
#include "FontManager.h"        // for g_pFontManager
#include "CameraManager.h"      // for g_pCameraManager
#include "ICamera.h"            // for g_pCurrentCamera
#include "MapManager.h"         // for g_pMapManager
#include "IMap.h"               // for g_pCurrentMap
#include "keyManager.h"         // for g_pKeyManager
#include "UIManager.h"          // for g_pUIManager
#include "CollisionManager.h"   // for g_pCollisionManager
#include "SoundManager.h"       // for g_pSoundManager
// end manager classes

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