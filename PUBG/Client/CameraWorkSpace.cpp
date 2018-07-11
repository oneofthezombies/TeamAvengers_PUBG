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
    m_look = m_eye + Vector3::RIGHT;
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
        D3DXMATRIX m;
        D3DXMatrixTranslation(&m, 0.0f, 8.0f, 0.0f);
        if (pTarInfo->pGunBolt)
            m_worldMatrix = m * pTarInfo->pGunBolt->CombinedTransformationMatrix * pTarInfo->pHandGun->CombinedTransformationMatrix *  pTarInfo->pTransform->GetTransformationMatrix();
        else
            cout << "없어!" << endl;
    }
    m_eye = Vector3::ZERO;
    //D3DXVECTOR3 eye 
    m_look = m_eye + Vector3::RIGHT;
}

void CameraOnGun::Render()
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
