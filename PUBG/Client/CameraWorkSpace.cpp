#include "stdafx.h"
#include "ICamera.h"
#include "ComponentTransform.h"
#include "Character.h"
#include "Ray.h"
#include "HeightMap.h"

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
    //70 Degrees FP sight
    m_fovY = D3DX_PI * (70.0f / 180.0f);

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
    //견착하는 부분은 3인칭에서만 있기에
    if (Input()()->IsOnceKeyDown(VK_RBUTTON))
        Camera()()->SetCurrentCamera(TAG_CAMERA::KyunChak);

    
    Character::Info* pTarInfo = GetTargetInfo();
    if (!pTarInfo)
    {
        D3DXMatrixIdentity(&m_worldMatrix);
    }
    else
    {
        D3DXMATRIX tarR, baseT;
        D3DXVECTOR3 vRot = *pTarInfo->pRotationForCamera;
        Debug << "*pTarInfo->pRotationForCamera : " << *pTarInfo->pRotationForCamera << endl << endl;
        D3DXMatrixRotationYawPitchRoll(&tarR, 0.0f, vRot.x, 0.0f);

        D3DXMatrixTranslation(&baseT, 0.0f, 30.0f, 0.0f);
        


        //*Important!  (model space)                      (rotation get from character)    (char transformation + height up till head)
        m_worldMatrix = 
            baseT
            * tarR
            * pTarInfo->pHead->CombinedTransformationMatrix
            * pTarInfo->pTransform->GetTransformationMatrix()

            
            ;//baseT;

    }
    




}

void ICamera::UpdateViewProjMatrix()
{

    D3DXVECTOR3 eye = Vector3::ZERO;
    D3DXVECTOR3 look = eye + -(D3DXVECTOR3(0, -2, 0));
    ////-------------------------------------------------
    //Character::Info* pTarInfo = GetTargetInfo();
    //if (!pTarInfo)
    //{
    //    D3DXMatrixIdentity(&m_worldMatrix);
    //}
    //else
    //{
    //    D3DXMATRIX tarR, baseT;
    //    D3DXVECTOR3 vRot = *pTarInfo->pRotationForCamera;
    //    D3DXMatrixRotationYawPitchRoll(&tarR, vRot.y, vRot.x, vRot.z);
    //    D3DXMatrixTranslation(&baseT, m_position.x, m_position.y, m_position.z);
    //    baseT *= pTarInfo->pTransform->GetTransformationMatrix();
    //    //*Important!  (model space)                      (rotation get from character)    (char transformation + height up till head)
    //    m_worldMatrix = pTarInfo->pTPP->CombinedTransformationMatrix    *    tarR    *      baseT;
    //}
    ////-------------------------------------------------
    D3DXVec3TransformCoord(&eye, &eye, &m_worldMatrix);
    D3DXVec3TransformCoord(&look, &look, &m_worldMatrix);
    D3DXMatrixLookAtLH(&m_viewMatrix, &eye, &look, &Vector3::UP);

    auto pD = Device()();
    pD->SetTransform(D3DTS_VIEW, &m_viewMatrix);
    if (m_tagCamera != TAG_CAMERA::Scope2X)
    {
        RECT rc;
        GetClientRect(g_hWnd, &rc);
        D3DXMatrixPerspectiveFovLH(&m_projectionMatrix,
            m_fovY, static_cast<float>(rc.right) / static_cast<float>(rc.bottom),
            1.0f, 5000.0f);
        pD->SetTransform(D3DTS_PROJECTION, &m_projectionMatrix);
    }

}