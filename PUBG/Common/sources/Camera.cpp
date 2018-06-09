#include "stdafx.h"
#include "ICamera.h"

ICamera::ICamera()
    : m_rotX(0.0f)
    , m_rotY(0.0f)
    , m_isALTbuttonStay(false)
    , m_pTargetPos(nullptr)
    , m_pTargetRot(nullptr)
{
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    m_aspect = rc.right / (float)rc.bottom;
    m_up = D3DXVECTOR3(0, 1, 0);
}

void ICamera::Init()
{
    m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
    m_lookAt = D3DXVECTOR3(m_eye.x, m_eye.y, m_eye.z+1);

    //ĳ���ͷκ��� pos�� rot �޾ƿ�
    m_pTargetPos = g_pCameraManager->GetTargetPos();
    m_pTargetRot = g_pCameraManager->GetTargetRot();
    //if (!m_pTargetPos || !m_pTargetRot)
    //    Debug->ShowMessageBox();

    //m_ptPrevMouse;
    D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_lookAt, &m_up);
    g_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

    D3DXMatrixPerspectiveFovLH(&m_matProj, m_fovY, m_aspect, 1, 1000);
    g_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void ICamera::Update()
{
    D3DXMATRIX matWorld,matR, matT;
    D3DXMatrixIdentity(&matWorld);

    m_pTargetPos = g_pCameraManager->GetTargetPos();
    m_pTargetRot = g_pCameraManager->GetTargetRot();
    //if(!m_pTargetPos||!m_pTargetRot)
    //    Debug->ShowMessageBox();

    m_eye = D3DXVECTOR3(m_basePosX, m_basePosY, -m_distance);
    m_lookAt = D3DXVECTOR3(m_eye.x, m_eye.y, m_eye.z + 1);
    m_eyeBackRay = D3DXVECTOR3(m_basePosX, m_basePosY, 0);  //CamEye���� dist��ŭ ������ �Ÿ����� Ray�� ��� ����

    if (!m_isALTbuttonStay) //Alt�� ������ �ʾ�����
    {
        if (m_pTargetRot)
            //����� ĳ���Ϳ��� ���� m_pos �� m_rot�� ����ͼ� ���
            D3DXMatrixRotationYawPitchRoll(&matR, m_pTargetRot->y, m_pTargetRot->x, 0.0f/*pTargetRot->z*/);
        else
            D3DXMatrixRotationYawPitchRoll(&matR, 0.0f, 0.0f, 0.0f/*pTargetRot->z*/);
    }
    else//ALT�� ���ȴ����
        //ī�޶󿡼� ���� rotX rotY �� + ĳ���Ϳ��� ���� m_pos
        D3DXMatrixRotationYawPitchRoll(&matR, m_rotY, m_rotX, 0.0f);
    
    if (m_pTargetPos)
        D3DXMatrixTranslation(&matT, m_pTargetPos->x, m_pTargetPos->y, m_pTargetPos->z);
    else
        D3DXMatrixIdentity(&matT);

    matWorld = matWorld * matR * matT;
    D3DXVec3TransformCoord(&m_eye, &m_eye, &matWorld);
    D3DXVec3TransformCoord(&m_lookAt, &m_lookAt, &matWorld);
    D3DXVec3TransformCoord(&m_eyeBackRay, &m_eyeBackRay, &matWorld);
    m_eyeDir = m_lookAt - m_eye;    //eye�� ���� ���� Update;
    D3DXVec3Normalize(&m_eyeDir, &m_eyeDir); //normalize

    D3DXMatrixLookAtLH(&m_matView, &m_eye, &m_lookAt, &m_up);
    g_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

}

CameraState::CameraState ICamera::GetState() const
{
    return m_cameraState;
}

