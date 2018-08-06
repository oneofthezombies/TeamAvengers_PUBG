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
    m_cameras.emplace(TAG_CAMERA::OnGun, new CameraOnGun);
    m_cameras.emplace(TAG_CAMERA::KyunChak, new CameraKyunChak);
    //m_cameras.emplace(TAG_CAMERA::Scope2X, new Camera2xScope);
    m_cameras.emplace(TAG_CAMERA::Lobby, new CameraLobby);

    //SetCurrentCamera(TAG_CAMERA::Default);
    //SetCurrentCamera(TAG_CAMERA::Third_Person);
    SetCurrentCamera(TAG_CAMERA::Lobby);
}

void CameraManager::Destroy()
{
    for (auto c : m_cameras)
        SAFE_DELETE(c.second);
}

void CameraManager::Update()
{
    InputManager*  pInput = Input()();

    if (pCurrentCamera->GetTagCamera() != TAG_CAMERA::Lobby)
    {
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
    }

    //if (pInput->IsOnceKeyDown('L'))
    //{
    //    SetCurrentCamera(TAG_CAMERA::OnGun);
    //}


    if (pCurrentCamera)
    {
        ////for debug
        //if (pCurrentCamera->GetTagCamera() != TAG_CAMERA::OnGun)
        //    m_cameras[TAG_CAMERA::OnGun]->Update();

        pCurrentCamera->Update();
        
        if (pCurrentCamera->GetTagCamera() != TAG_CAMERA::Default) //디버그 카메라는 player의 이동에 영향이 없도록
            pCurrentCamera->UpdateViewProjMatrix();

        pCurrentCamera->UpdateFrustumCulling(); // origin
        //// for recording
        //if (GetAsyncKeyState('7') & 0x0001)
        //{
        //    pCurrentCamera->UpdateFrustumCulling();
        //}

        //for debug
        //m_cameras[TAG_CAMERA::OnGun]->Render();
        if (pCurrentCamera->GetTagCamera() == TAG_CAMERA::KyunChak)
        {
            CameraKyunChak* pCam = static_cast<CameraKyunChak*>(pCurrentCamera);
            if (pCam->GetDurTime() < 0.0f)
                SetCurrentCamera(TAG_CAMERA::Third_Person);
        }
    }
}

void CameraManager::Render()
{
    if (!Collision()()->IsRender()) return;

    if (pCurrentCamera) pCurrentCamera->CameraRender();
}

void CameraManager::SetTarget(Character::Info* info)
{
    assert(info &&
        "CameraManager::SetTarget(), pointer is null.");
    m_targetInfo = info;
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
