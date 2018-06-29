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
    m_cameras.emplace(TAG_CAMERA::KyunChak, new CameraKyunChak);
    //m_cameras.emplace(TAG_CAMERA::Scope2X, new Camera2xScope);

    //SetCurrentCamera(TAG_CAMERA::Default);
    SetCurrentCamera(TAG_CAMERA::Third_Person);
}

void CameraManager::Destroy()
{
    for (auto c : m_cameras)
        SAFE_DELETE(c.second);
}

void CameraManager::Update()
{
    InputManager*  pInput = Input()();

    if (pInput->IsOnceKeyUp('P'))
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

        if(pCurrentCamera->GetTagCamera()!=TAG_CAMERA::Default)//디버그 카메라는 player의 이동에 영향이 없도록
          pCurrentCamera->UpdateViewProjMatrix();
        
        if (Input()()->IsOnceKeyUp(VK_SPACE)) //눌렀을때 볼 수 있도록
        {
            pCurrentCamera->UpdateFrustumCulling();
        }
        
    }
}

void CameraManager::SetTarget(Transform* pTarget, D3DXVECTOR3* pTargetRotForCameraTP)
{
    assert(pTarget && pTargetRotForCameraTP && 
        "CameraManager::SetTarget(), pointer is null.");

    m_targetTransform.pTransform = pTarget;
    m_targetTransform.pRotForCameraTP = pTargetRotForCameraTP;
}

void CameraManager::SetCurrentCamera(const TAG_CAMERA tag)
{
    const auto search = m_cameras.find(tag);
    assert(search != m_cameras.end() && 
        "CameraManager::SetCurrentCamera(), not found camera.");

    pCurrentCamera = search->second;
    pCurrentCamera->Reset();
}

TargetTransform::TargetTransform()
    : pTransform(nullptr)
    , pRotForCameraTP(nullptr)
{
}
