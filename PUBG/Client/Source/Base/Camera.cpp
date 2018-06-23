#include "stdafx.h"
#include "Camera.h"

//ICamera::ICamera()
//    : m_rotX(0.0f)
//    , m_rotY(0.0f)
//    , m_isALTbuttonStay(false)
//    , m_pTargetPos(nullptr)
//    , m_pTargetRot(nullptr)
//{
//    RECT rc;
//    GetClientRect(g_hWnd, &rc);
//    m_aspect = rc.right / (float)rc.bottom;
//    m_up = D3DXVECTOR3(0, 1, 0);
//}
//
//void ICamera::Init()
//{
//    m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
//    m_lookAt = D3DXVECTOR3(m_eye.x, m_eye.y, m_eye.z+1);
//
//    //캐릭터로부터 pos와 rot 받아옴
//    m_pTargetPos = g_pCameraManager->GetTargetPos();
//    m_pTargetRot = g_pCameraManager->GetTargetRot();
//    //if (!m_pTargetPos || !m_pTargetRot)
//    //    Debug->ShowMessageBox();
//
//    //m_ptPrevMouse;
//    D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_lookAt, &m_up);
//    g_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
//
//    D3DXMatrixPerspectiveFovLH(&m_matProj, m_fovY, m_aspect, 1, 1000);
//    g_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
//}
//
//void ICamera::Update()
//{
//    D3DXMATRIX matWorld,matR, matT;
//    D3DXMatrixIdentity(&matWorld);
//
//    m_pTargetPos = g_pCameraManager->GetTargetPos();
//    m_pTargetRot = g_pCameraManager->GetTargetRot();
//    //if(!m_pTargetPos||!m_pTargetRot)
//    //    Debug->ShowMessageBox();
//
//    m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
//    m_lookAt = D3DXVECTOR3(m_eye.x, m_eye.y, m_eye.z + 1);
//    m_eyeBackRay = D3DXVECTOR3(m_basePosX, m_basePosY, 0);  //CamEye에서 dist만큼 떨어진 거리에서 Ray를 쏘기 위함
//
//    if (!m_isALTbuttonStay) //Alt가 눌리지 않았으면
//    {
//        if (m_pTargetRot)
//            //여기는 캐릭터에서 받은 m_pos 와 m_rot을 갖고와서 계산
//            D3DXMatrixRotationYawPitchRoll(&matR, m_pTargetRot->y, m_pTargetRot->x, 0.0f/*pTargetRot->z*/);
//        else
//            D3DXMatrixRotationYawPitchRoll(&matR, 0.0f, 0.0f, 0.0f/*pTargetRot->z*/);
//    }
//    else//ALT가 눌렸더라면
//        //카메라에서 받은 rotX rotY 값 + 캐릭터에서 받은 m_pos
//        D3DXMatrixRotationYawPitchRoll(&matR, m_rotY, m_rotX, 0.0f);
//    
//    if (m_pTargetPos)
//        D3DXMatrixTranslation(&matT, m_pTargetPos->x, m_pTargetPos->y, m_pTargetPos->z);
//    else
//        D3DXMatrixIdentity(&matT);
//
//    matWorld = matWorld * matR * matT;
//    D3DXVec3TransformCoord(&m_eye, &m_eye, &matWorld);
//    D3DXVec3TransformCoord(&m_lookAt, &m_lookAt, &matWorld);
//    D3DXVec3TransformCoord(&m_eyeBackRay, &m_eyeBackRay, &matWorld);
//    m_eyeDir = m_lookAt - m_eye;    //eye가 보는 방향 Update;
//    D3DXVec3Normalize(&m_eyeDir, &m_eyeDir); //normalize
//
//    D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_lookAt, &m_up);
//    g_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
//
//}
//
//CameraState::CameraState ICamera::GetState() const
//{
//    return m_cameraState;
//}

Camera::Camera()
{
    m_Direction.z = 1.0f;
    m_Position.y = 88.0f;
    m_Position.z = -268.0f;
    D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &(m_Position + m_Direction), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
    g_pDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, D3DX_PI * 0.25f, static_cast<float>(rc.right) / static_cast<float>(rc.bottom), 1.0f, 1000.0f);
    g_pDevice->SetTransform(D3DTS_PROJECTION, &m_ProjectionMatrix);
}

Camera::~Camera()
{
}

void Camera::Update()
{
    bool isUpdated = false;
    //if (g_pInput->IsStayKeyDown('W'))
    //{
    //    m_Position.y += 1.0f;
    //    isUpdated = true;
    //}

    //if (g_pInput->IsStayKeyDown('S'))
    //{
    //    m_Position.y -= 1.0f;
    //    isUpdated = true;
    //}

    //if (g_pInput->IsStayKeyDown('A'))
    //{
    //    m_Position.x -= 1.0f;
    //    isUpdated = true;
    //}

    //if (g_pInput->IsStayKeyDown('D'))
    //{
    //    m_Position.x += 1.0f;
    //    isUpdated = true;
    //}

    //if (g_pInput->IsStayKeyDown('Q'))
    //{
    //    m_Position.z += 0.5f;
    //    isUpdated = true;
    //}

    //if (g_pInput->IsStayKeyDown('E'))
    //{
    //    m_Position.z -= 0.5f;
    //    isUpdated = true;
    //}

    if (isUpdated)
    {
        D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &(m_Position + m_Direction), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
        g_pDevice->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    }

    Debug << "Camera position : " << m_Position << '\n';
}

const D3DXMATRIX& Camera::GetViewMatrix()
{
    return m_ViewMatrix;
}

const D3DXMATRIX& Camera::GetProjectionMatrix()
{
    return m_ProjectionMatrix;
}
