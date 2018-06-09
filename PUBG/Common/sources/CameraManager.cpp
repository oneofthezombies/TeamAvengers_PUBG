#include "stdafx.h"
#include "CameraManager.h"
#include "ICamera.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"


CameraManager::CameraManager()
    : m_pTargetDir(nullptr)
    , m_pTargetPos(nullptr)
{
    m_pCurrentCamera = NULL;
}


CameraManager::~CameraManager()
{
}

void CameraManager::SetCurrentCamera(int cameraState)
{
    m_pCurrentCamera = m_mapList[cameraState];
    m_pCurrentCamera->Init(); 
    ////�ϴ� switch ������ �ذ�
    //switch (m_pCurrentCamera->m_cameraState)
    //{
    //case CameraState::FIRSTPERSON:
    //    static_cast<FirstPersonCamera*>(m_pCurrentCamera)->Init();
    //    break;
    //case CameraState::THIRDPERSON :
    //    static_cast<ThirdPersonCamera*>(m_pCurrentCamera)->Init();
    //    break;
    //case CameraState::FP2TP :
    //    static_cast<CameraFPToTP*>(m_pCurrentCamera)->Init();
    //    break;
    //case CameraState::TP2FP :
    //    static_cast<CameraTPToFP*>(m_pCurrentCamera)->Init();
    //    break;
    //case CameraState::KYUNCHAK:
    //    static_cast<CameraKyunChak*>(m_pCurrentCamera)->Init();
    //    break;
    //}
    
}

void CameraManager::Init()
{
    ICamera* tempContainer;
    //FP �� TP ���� + Map �� �߰�
    tempContainer = new FirstPersonCamera();
    AddCamera(CameraState::FIRSTPERSON, tempContainer);
    tempContainer = new ThirdPersonCamera();
    AddCamera(CameraState::THIRDPERSON, tempContainer);
    
    //FP�� TP�� lerp�� ġȯ�� �ִ� class ����
    tempContainer = new CameraFPToTP();
    AddCamera(CameraState::FP2TP, tempContainer);
    tempContainer = new CameraTPToFP();
    AddCamera(CameraState::TP2FP, tempContainer);

    //����
    tempContainer = new CameraKyunChak();
    AddCamera(CameraState::KYUNCHAK, tempContainer);

    //2���� 4���� ������
    tempContainer = new Camera2xScope();
    AddCamera(CameraState::SCOPE2X, tempContainer);
    tempContainer = new Camera4xScope();
    AddCamera(CameraState::SCOPE4X, tempContainer);

    //currCamera ���� + init()
    SetCurrentCamera(CameraState::THIRDPERSON);
}

void CameraManager::Destroy()
{
    for (auto& c : m_mapList)
    {
        SAFE_RELEASE(c.second);
    }
}

void CameraManager::Update()
{
    const float dt = g_pTimeManager->GetDeltaTime();

    //V�� �������� 3��Ī�̸� 1��Ī����, 1��Ī�̸� 3��Ī����
    if (g_pKeyManager->IsOnceKeyDown('V'))
    {
        if (m_pCurrentCamera->m_cameraState == CameraState::THIRDPERSON)
        {
            g_pCameraManager->SetCurrentCamera(CameraState::TP2FP);
        }
        else
        {
            g_pCameraManager->SetCurrentCamera(CameraState::FP2TP);
        }
    }
    SAFE_UPDATE(m_pCurrentCamera);
    Debug->AddText("CurrentCameraState : ");
    Debug->AddText(m_pCurrentCamera->m_cameraState);
    Debug->EndLine();
}

void CameraManager::SetTarget(D3DXVECTOR3& pos, D3DXVECTOR3& dir)
{
    m_pTargetPos = &pos;
    m_pTargetDir = &dir;
}

D3DXVECTOR3* CameraManager::GetTargetPos() const
{
    return m_pTargetPos;
}

D3DXVECTOR3* CameraManager::GetTargetRot() const
{
    return m_pTargetDir;
}

D3DXVECTOR3 CameraManager::GetCurrentCameraDir() 
{ 
    return m_pCurrentCamera->GetEyeDir(); 
}

D3DXVECTOR3 CameraManager::GetCurrentCameraEye() 
{ 
    return m_pCurrentCamera->GetEyePos(); 
}