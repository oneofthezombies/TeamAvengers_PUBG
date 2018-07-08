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
    m_eye = Vector3::ZERO;
    //D3DXVECTOR3 eye 
    m_look = m_eye + (D3DXVECTOR3(0, 1, 0));


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
        D3DXMatrixRotationYawPitchRoll(&tarR, vRot.y, vRot.x, 0.0f);

        D3DXMATRIX baseY, baseZ;
        D3DXMatrixTranslation(&baseY, -20.0f, 180.0f, 0.0f);
        D3DXMatrixTranslation(&baseZ, 0.0f, 0.0f, 75.0f);
        m_worldMatrix 
            = baseZ 
            * tarR
            * baseY
            * pTarInfo->pTransform->GetTransformationMatrix();



        
    }

    m_eye = Vector3::ZERO;
    m_look = m_eye - Vector3::FORWARD;
    
    
    int i = 0;
}

void CameraThirdPerson::Render()
{
    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr, 
        Resource()()->GetBoundingSphereMesh(), 
        0, 
        [this](LPD3DXEFFECT pEffect) 
    {
        D3DXMATRIX s, m;
        D3DXMatrixScaling(&s, 10.0f, 10.0f, 10.0f);
        m = s * m_worldMatrix;
        pEffect->SetMatrix(Shader::World, &m);

        D3DXCOLOR red(1.0f, 0.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &red, sizeof red);
    });
}

void ICamera::UpdateViewProjMatrix()
{

    D3DXVec3TransformCoord(&m_eye, &m_eye, &m_worldMatrix);
    D3DXVec3TransformCoord(&m_look, &m_look, &m_worldMatrix);
    D3DXMatrixLookAtLH(&m_viewMatrix, &m_eye, &m_look, &Vector3::UP);

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