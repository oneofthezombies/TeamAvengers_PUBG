#pragma once
#include "Singleton.h"
#include "ICamera.h"

class Transform;

class CameraManager : public Singleton<CameraManager>
{
private:
    unordered_map<TAG_CAMERA, ICamera*> m_cameras;

    ICamera*   pCurrentCamera;
    Transform* pTarget;

             CameraManager();
    virtual ~CameraManager();

public:
    void Init();
    void Destroy();
    void Update();

    void       SetTarget(Transform* pTarget);
    Transform* GetTarget();

    void     SetCurrentCamera(const TAG_CAMERA tag);
    ICamera* GetCurrentCamera();

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

////#define CameraChangeSpeed 6.0f
//class ICamera;
//
//class CameraManager : public Singleton<CameraManager>
//{
//private:
//    ICamera * m_pCurrentCamera;
//    map<int, ICamera*> m_mapList;
//
//    D3DXVECTOR3*	m_pTargetPos;
//    D3DXVECTOR3*    m_pTargetDir;
//
//public:
//    //이렇게 까지 할 필요가 없을까? 단지 First와 Third만 사용할 것이기 때문에?
//    //그럼 enum은 어디에다가 넣어야 하는가?
//    void AddCamera(int cameraState, ICamera* pCamera)
//    {
//        if (m_mapList.count(cameraState) == 1)
//            return;
//        m_mapList[cameraState] = pCamera;
//    }
//    void SetCurrentCamera(int cameraState);
//    ICamera* GetCurrentCamera() { return m_pCurrentCamera; }
//    D3DXVECTOR3 GetCurrentCameraDir();
//    D3DXVECTOR3 GetCurrentCameraEye();
//
//    void Init();    
//    void Destroy();
//    void Update();
//    void SetTarget(D3DXVECTOR3& pos, D3DXVECTOR3& dir);
//    D3DXVECTOR3* GetTargetPos() const;
//    D3DXVECTOR3* GetTargetRot() const;
//    
//    //JH---------------
//public:
//    vector<D3DXVECTOR3> m_vecVertex_sample;
//    void SetWall(vector<D3DXVECTOR3> vecVertex_sample)
//    {
//        m_vecVertex_sample = vecVertex_sample;
//    }
//    vector<D3DXVECTOR3> GetWall()
//    {
//        return m_vecVertex_sample;
//    }
//    //------------------
//
//	friend Singleton<CameraManager>;
//};

