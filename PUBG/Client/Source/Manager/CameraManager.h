#pragma once
#include "Singleton.h"
#include "ICamera.h"

#define g_pCamera        Camera       ()()
#define g_pCurrentCamera CurrentCamera()()
class Transform;
struct TargetTransform
{
    Transform*   pTransform;
    D3DXVECTOR3* pRotForCameraTP;

    TargetTransform();
};
class CameraManager : public Singleton<CameraManager>
{
private:
    unordered_map<TAG_CAMERA, ICamera*> m_cameras;

    ICamera*   pCurrentCamera;
    TargetTransform m_targetTransform;

             CameraManager();
    virtual ~CameraManager();

public:
    void Init();
    void Destroy();
    void Update();

    void             SetTarget(Transform* pTarget, D3DXVECTOR3* pTargetRotForCameraTP);
    TargetTransform* GetTarget();

    void     SetCurrentCamera(const TAG_CAMERA tag);
    ICamera* GetCurrentCamera();

    TargetTransform* GetTargetTransformPtr();

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
