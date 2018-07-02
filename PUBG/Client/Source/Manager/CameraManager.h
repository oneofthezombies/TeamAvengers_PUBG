#pragma once
#include "Singleton.h"
#include "ICamera.h"    //ÈÆÈ¸Çü! struct °¡ ¾È¸ÔÇô¼­ ÀÌ·¸°Ô ³Ö¾ú´Âµ¥ ±¦Âú³ª¿ä?

#include "Character.h"

#define g_pCamera        Camera       ()()
#define g_pCurrentCamera CurrentCamera()()


class CameraManager : public Singleton<CameraManager>
{
private:
    unordered_map<TAG_CAMERA, ICamera*> m_cameras;

    ICamera*   pCurrentCamera;
    Character::Info* m_targetInfo;

             CameraManager();
    virtual ~CameraManager();

public:
    void Init();
    void Destroy();
    void Update();

    void  SetTarget(Character::Info* info);
    Character::Info* GetTargetInfo();

    void     SetCurrentCamera(const TAG_CAMERA tag);
    ICamera* GetCurrentCamera() { assert(pCurrentCamera &&"CameraManager::GetCurrentCamera(), current camera is null."); return pCurrentCamera; }


    friend Singleton<CameraManager>;
};

struct Camera
{
    CameraManager* operator()()
    {
        return CameraManager::GetInstance();
    }
};

struct CurrentCamera
{
    ICamera* operator()()
    {
        return CameraManager::GetInstance()->GetCurrentCamera();
    }
};
