#include "stdafx.h"
#include "ICamera.h"
#include "ComponentTransform.h"
#include "Character.h"

void ICamera::UpdateViewProjMatrix()
{
    D3DXVECTOR3 eye = Vector3::ZERO;
    D3DXVECTOR3 look = eye + -Vector3::FORWARD;

    D3DXMATRIX camT, tarR, tarWorld, world;

    D3DXMatrixTranslation(&camT, m_position.x, m_position.y, m_position.z);

    TargetTransform* pTar = GetTarget();
    if (!pTar || !pTar->pRotForCameraTP)
    {
        D3DXMatrixIdentity(&tarR);
        D3DXMatrixIdentity(&tarWorld);
    }
    else
    {
        const D3DXVECTOR3 rot = *pTar->pRotForCameraTP;
        D3DXMatrixRotationYawPitchRoll(&tarR, rot.y, rot.x, rot.z);

        tarWorld = pTar->pTransform->GetTransformationMatrix();
    }

    world = camT * tarR * tarWorld;
    D3DXVec3TransformCoord(&eye, &eye, &world);
    D3DXVec3TransformCoord(&look, &look, &world);

    D3DXMatrixLookAtLH(&m_viewMatrix, &eye, &look, &Vector3::UP);

    auto pD = Device()();
    pD->SetTransform(D3DTS_VIEW, &m_viewMatrix);

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    D3DXMatrixPerspectiveFovLH(&m_projectionMatrix,
        m_fovY, static_cast<float>(rc.right) / static_cast<float>(rc.bottom),
        1.0f, 5000.0f);
    pD->SetTransform(D3DTS_PROJECTION, &m_projectionMatrix);
}


//-----------------------------------------------------------------
CameraFirstPerson::CameraFirstPerson()
    : ICamera(TAG_CAMERA::First_Person)
{

}

CameraFirstPerson::~CameraFirstPerson()
{
}

void CameraFirstPerson::Reset()
{
    m_position = D3DXVECTOR3(FP_BASEPOSX, FP_BASEPOSY, FP_DISTANCE);
    //70 Degrees FP sight
    m_fovY = D3DX_PI * (70.0f / 180.0f);

    //D3DXQuaternionRotationYawPitchRoll(&m_quarernion, m_rotation.y, m_rotation.x, m_rotation.z);
}

void CameraFirstPerson::Update()
{
    //RBUTTON Up 하면 다시 TP 로 돌아가기
    if (Input()()->IsOnceKeyUp(VK_RBUTTON))
        Camera()()->SetCurrentCamera(TAG_CAMERA::KyunChak);
}
