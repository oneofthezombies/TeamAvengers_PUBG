#include "stdafx.h"
#include "ICamera.h"
#include "ComponentTransform.h"
#include "Character.h"
#include "Ray.h"
#include "HeightMap.h"
CameraFirstPerson::CameraFirstPerson(const TAG_CAMERA tag)
    : ICamera(tag)
{

}

CameraFirstPerson::~CameraFirstPerson()
{
}

void CameraFirstPerson::Reset()
{
    //70 Degrees FP sight
    m_fovY = D3DX_PI * (70.0f / 180.0f);
    m_eye = Vector3::ZERO;
    //D3DXVECTOR3 eye 
    m_look = m_eye + (D3DXVECTOR3(0, 1, 0));

}

void CameraFirstPerson::Update()
{
    //RBUTTON Up 하면 다시 TP 로 돌아가기
    //if (Input()()->IsOnceKeyUp(VK_RBUTTON))
    //    Camera()()->SetCurrentCamera(TAG_CAMERA::KyunChak);


    Character::Info* pTarInfo = GetTargetInfo();
    if (!pTarInfo)
    {
        D3DXMatrixIdentity(&m_worldMatrix);
    }
    else
    {
        //*Important!          (model space)                                            (char transformation )
        m_worldMatrix = pTarInfo->pFPP->CombinedTransformationMatrix * pTarInfo->pTransform->GetTransformationMatrix();
    }

    m_eye = Vector3::ZERO;
    //D3DXVECTOR3 eye 
    m_look = m_eye + D3DXVECTOR3(0, 1, 0);

}



CameraOnGun::CameraOnGun()
    : ICamera(TAG_CAMERA::OnGun)
{
}
CameraOnGun::~CameraOnGun()
{
}
void CameraOnGun::Reset()
{
    m_fovY = D3DX_PI * (70.0f / 180.0f);
    m_eye = Vector3::ZERO;
    m_look = m_eye + Vector3::UP;
}
void CameraOnGun::Update()
{
    Character::Info* pTarInfo = GetTargetInfo();
    if (!pTarInfo)
    {
        D3DXMatrixIdentity(&m_worldMatrix);
    }
    else
    {
        //D3DXMATRIX m;
        //D3DXMatrixTranslation(&m, 0.0f, 0.0f, -20.0f);
        m_worldMatrix = /*m **/ pTarInfo->pHand->CombinedTransformationMatrix * pTarInfo->pTransform->GetTransformationMatrix();
    }
    m_eye = Vector3::ZERO;
    //D3DXVECTOR3 eye 
    m_look = m_eye + Vector3::UP;
}
