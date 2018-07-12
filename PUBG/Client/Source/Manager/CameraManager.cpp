#include "stdafx.h"
#include "CameraManager.h"

CameraManager::CameraManager()
    : Singleton<CameraManager>()
    , pCurrentCamera(nullptr)
    , m_targetInfo(nullptr)
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
    //m_cameras.emplace(TAG_CAMERA::KyunChak, new CameraKyunChak);
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

    //Debug camera
    if (pInput->IsOnceKeyDown('P'))
    {
        if (pCurrentCamera->GetTagCamera() != TAG_CAMERA::Default)
            SetCurrentCamera(TAG_CAMERA::Default);
        else
            SetCurrentCamera(TAG_CAMERA::Third_Person);
    }
    //시점 변환
    if (pInput->IsOnceKeyDown('V'))
    {
        if (pCurrentCamera->GetTagCamera() != TAG_CAMERA::Third_Person)
            SetCurrentCamera(TAG_CAMERA::Third_Person);
        else
            SetCurrentCamera(TAG_CAMERA::First_Person);
    }


    if (pCurrentCamera)
    {
        //for debug
        if (pCurrentCamera->GetTagCamera() != TAG_CAMERA::Third_Person)
            m_cameras[TAG_CAMERA::Third_Person]->Update();

        pCurrentCamera->Update();

        if(pCurrentCamera->GetTagCamera()!=TAG_CAMERA::Default)//디버그 카메라는 player의 이동에 영향이 없도록
          pCurrentCamera->UpdateViewProjMatrix();

        //if (Input()()->IsOnceKeyUp(VK_SPACE)) //눌렀을때 볼 수 있도록
        //{
        //    pCurrentCamera->UpdateFrustumCulling();
        //}
        //pCurrentCamera->CameraRender();

        CurrentCamera()()->UpdateFrustumCulling();


        //for debug
        m_cameras[TAG_CAMERA::Third_Person]->Render();
    }
}

void CameraManager::SetTarget(Character::Info* info/*Transform* pTarget, D3DXVECTOR3* pTargetRotForCameraTP*/)
{
    assert(info &&
        "CameraManager::SetTarget(), pointer is null.");
    m_targetInfo = info;
    //m_targetTransform.pTransform = pTarget;
    //m_targetTransform.pRotForCameraTP = pTargetRotForCameraTP;
}

Character::Info * CameraManager::GetTargetInfo()
{
    return m_targetInfo;
}

void CameraManager::SetCurrentCamera(const TAG_CAMERA tag)
{
    const auto search = m_cameras.find(tag);
    assert(search != m_cameras.end() && 
        "CameraManager::SetCurrentCamera(), not found camera.");

    pCurrentCamera = search->second;
    pCurrentCamera->Reset();
}
