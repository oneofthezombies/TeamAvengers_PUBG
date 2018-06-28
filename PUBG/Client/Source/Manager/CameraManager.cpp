#include "stdafx.h"
#include "CameraManager.h"

CameraManager::CameraManager()
    : Singleton<CameraManager>()
    , pCurrentCamera(nullptr)
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::Init()
{
    m_cameras.emplace(TAG_CAMERA::Default, new CameraFree);
    m_cameras.emplace(TAG_CAMERA::First_Person, new CameraFirstPerson);
    m_cameras.emplace(TAG_CAMERA::Third_Person, new CameraThirdPerson);
    
    //SetCurrentCamera(TAG_CAMERA::Default);
    SetCurrentCamera(TAG_CAMERA::Default);
}

void CameraManager::Destroy()
{
    for (auto c : m_cameras)
        SAFE_DELETE(c.second);
}

void CameraManager::Update()
{
    if (Input()()->IsOnceKeyUp('P'))
    {
        if (pCurrentCamera->GetTagCamera() == TAG_CAMERA::Default)
        {
            SetCurrentCamera(TAG_CAMERA::Third_Person);
        }
        else 
        {
            SetCurrentCamera(TAG_CAMERA::Default);
        }

    }
        


    if (pCurrentCamera)
    {
        pCurrentCamera->Update();
        pCurrentCamera->UpdateViewProjMatrix();
    }
}

void CameraManager::SetTarget(Transform* pTarget, D3DXVECTOR3* pTargetRotForCameraTP)
{
    assert(pTarget && pTargetRotForCameraTP && 
        "CameraManager::SetTarget(), pointer is null.");

    m_targetTransform.pTransform = pTarget;
    m_targetTransform.pRotForCameraTP = pTargetRotForCameraTP;
}

TargetTransform* CameraManager::GetTarget()
{
    return &m_targetTransform;
}

void CameraManager::SetCurrentCamera(const TAG_CAMERA tag)
{
    const auto search = m_cameras.find(tag);
    assert(search != m_cameras.end() && 
        "CameraManager::SetCurrentCamera(), not found camera.");

    pCurrentCamera = search->second;
    pCurrentCamera->Reset();
}

ICamera* CameraManager::GetCurrentCamera()
{
    assert(pCurrentCamera && 
        "CameraManager::GetCurrentCamera(), current camera is null.");

    return pCurrentCamera;
}

TargetTransform* CameraManager::GetTargetTransformPtr()
{
    return &m_targetTransform;
}

TargetTransform::TargetTransform()
    : pTransform(nullptr)
    , pRotForCameraTP(nullptr)
{
}
