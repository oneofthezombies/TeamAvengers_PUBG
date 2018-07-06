#include "stdafx.h"
#include "ICamera.h"

//-----------------------------------------------------------------
CameraFirstPerson::CameraFirstPerson(const TAG_CAMERA tag)
    : ICamera(tag)
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
}

void CameraFirstPerson::Update()
{
    //RBUTTON Up �ϸ� �ٽ� TP �� ���ư���
    if (Input()()->IsOnceKeyUp(VK_RBUTTON))
        Camera()()->SetCurrentCamera(TAG_CAMERA::KyunChak);
}

//-----------------------------------------------------------------
CameraThirdPerson::CameraThirdPerson(const TAG_CAMERA tag)
    : ICamera(tag)
{
}

CameraThirdPerson::~CameraThirdPerson()
{
}

void CameraThirdPerson::Reset()
{
    m_position = D3DXVECTOR3(TP_BASEPOSX, TP_BASEPOSY, TP_DISTANCE);

    //80 Degrees TP sight
    m_fovY = D3DX_PI * (80.0f / 180.0f);

}

void CameraThirdPerson::Update()
{
    //�����ϴ� �κ��� 3��Ī������ �ֱ⿡
    if (Input()()->IsOnceKeyDown(VK_RBUTTON))
        Camera()()->SetCurrentCamera(TAG_CAMERA::KyunChak);
}