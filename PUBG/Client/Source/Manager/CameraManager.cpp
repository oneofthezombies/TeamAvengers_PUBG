#include "stdafx.h"
#include "CameraManager.h"

//#include "ICamera.h"
//#include "FirstPersonCamera.h"
//#include "ThirdPersonCamera.h"
//
//
//CameraManager::CameraManager()
//    : m_pTargetDir(nullptr)
//    , m_pTargetPos(nullptr)
//{
//    m_pCurrentCamera = NULL;
//}
//
//
//CameraManager::~CameraManager()
//{
//}
//
//void CameraManager::SetCurrentCamera(int cameraState)
//{
//    m_pCurrentCamera = m_mapList[cameraState];
//    m_pCurrentCamera->Init(); 
//    ////일단 switch 문으로 해결
//    //switch (m_pCurrentCamera->m_cameraState)
//    //{
//    //case CameraState::FIRSTPERSON:
//    //    static_cast<FirstPersonCamera*>(m_pCurrentCamera)->Init();
//    //    break;
//    //case CameraState::THIRDPERSON :
//    //    static_cast<ThirdPersonCamera*>(m_pCurrentCamera)->Init();
//    //    break;
//    //case CameraState::FP2TP :
//    //    static_cast<CameraFPToTP*>(m_pCurrentCamera)->Init();
//    //    break;
//    //case CameraState::TP2FP :
//    //    static_cast<CameraTPToFP*>(m_pCurrentCamera)->Init();
//    //    break;
//    //case CameraState::KYUNCHAK:
//    //    static_cast<CameraKyunChak*>(m_pCurrentCamera)->Init();
//    //    break;
//    //}
//    
//}
//
//void CameraManager::Init()
//{
//    ICamera* tempContainer;
//    //FP 와 TP 생성 + Map 에 추가
//    tempContainer = new FirstPersonCamera();
//    AddCamera(CameraState::FIRSTPERSON, tempContainer);
//    tempContainer = new ThirdPersonCamera();
//    AddCamera(CameraState::THIRDPERSON, tempContainer);
//    
//    //FP와 TP를 lerp로 치환해 주는 class 생성
//    tempContainer = new CameraFPToTP();
//    AddCamera(CameraState::FP2TP, tempContainer);
//    tempContainer = new CameraTPToFP();
//    AddCamera(CameraState::TP2FP, tempContainer);
//
//    //견착
//    tempContainer = new CameraKyunChak();
//    AddCamera(CameraState::KYUNCHAK, tempContainer);
//
//    //2배율 4배율 스코프
//    tempContainer = new Camera2xScope();
//    AddCamera(CameraState::SCOPE2X, tempContainer);
//    tempContainer = new Camera4xScope();
//    AddCamera(CameraState::SCOPE4X, tempContainer);
//
//    //currCamera 설정 + init()
//    SetCurrentCamera(CameraState::THIRDPERSON);
//}
//
//void CameraManager::Destroy()
//{
//    for (auto& c : m_mapList)
//    {
//        SAFE_RELEASE(c.second);
//    }
//}
//
//void CameraManager::Update()
//{
//    const float dt = g_pTimeManager->GetDeltaTime();
//
//    //V를 눌렀을때 3인칭이면 1인칭으로, 1인칭이면 3인칭으로
//    if (g_pKeyManager->IsOnceKeyDown('V'))
//    {
//        if (m_pCurrentCamera->m_cameraState == CameraState::THIRDPERSON)
//        {
//            g_pCameraManager->SetCurrentCamera(CameraState::TP2FP);
//        }
//        else
//        {
//            g_pCameraManager->SetCurrentCamera(CameraState::FP2TP);
//        }
//    }
//    SAFE_UPDATE(m_pCurrentCamera);
//    Debug->AddText("CurrentCameraState : ");
//    Debug->AddText(m_pCurrentCamera->m_cameraState);
//    Debug->EndLine();
//}
//
//void CameraManager::SetTarget(D3DXVECTOR3& pos, D3DXVECTOR3& dir)
//{
//    m_pTargetPos = &pos;
//    m_pTargetDir = &dir;
//}
//
//D3DXVECTOR3* CameraManager::GetTargetPos() const
//{
//    return m_pTargetPos;
//}
//
//D3DXVECTOR3* CameraManager::GetTargetRot() const
//{
//    return m_pTargetDir;
//}
//
//D3DXVECTOR3 CameraManager::GetCurrentCameraDir() 
//{ 
//    return m_pCurrentCamera->GetEyeDir(); 
//}
//
//D3DXVECTOR3 CameraManager::GetCurrentCameraEye() 
//{ 
//    return m_pCurrentCamera->GetEyePos(); 
//}

Camera* CameraManager::GetCurrentCamera()
{
    return &m_camera;
}
