#include "stdafx.h"
#include "ICamera.h"
#include "ComponentTransform.h"
#include "Character.h"

TargetTransform* ICamera::GetTarget()
{
    return Camera()()->GetTargetTransformPtr();
}

ICamera::ICamera(const TAG_CAMERA tag)
    : MemoryAllocator()
    , m_tagCamera(tag)
    , m_position(Vector3::ZERO)
    , m_rotation(Vector3::ZERO)
    , m_fovY(D3DX_PI * 0.5f)
{
    //D3DXQuaternionRotationYawPitchRoll(&m_quarernion, m_rotation.x, m_rotation.y, m_rotation.z);
}

ICamera::~ICamera()
{
}

void ICamera::UpdateViewProjMatrix()
{
    D3DXVECTOR3 eye = m_position;
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

    //D3DXVECTOR3 eye = m_position;
    //D3DXVECTOR3 look = eye + Vector3::FORWARD;
    //
    //D3DXMATRIX rotCam, rotTar, posTar, world;
    //D3DXMatrixRotationQuaternion(&rotCam, &m_quarernion);

    //auto pTarget = GetTarget();
    //if (!pTarget || !pTarget->pTransform)
    //{
    //    D3DXMatrixIdentity(&rotTar);
    //    D3DXMatrixIdentity(&posTar);
    //}
    //else
    //{
    //    Transform* tr = pTarget->pTransform;
    //    D3DXMatrixRotationQuaternion(&rotTar, &tr->GetRotation());
    //    D3DXVECTOR3 p = tr->GetPosition();
    //    D3DXMatrixTranslation(&posTar, p.x, p.y, p.z);
    //}

    //world = rotCam * rotTar * posTar;
    //D3DXVec3TransformCoord(&eye, &eye, &world);
    //D3DXVec3TransformCoord(&look, &look, &world);

    //D3DXMatrixLookAtLH(&m_viewMatrix, &eye, &look, &Vector3::UP);

    //auto pD = Device()();
    //pD->SetTransform(D3DTS_VIEW, &m_viewMatrix);

    //RECT rc;
    //GetClientRect(g_hWnd, &rc);
    //D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 
    //    m_fovY, static_cast<float>(rc.right) / static_cast<float>(rc.bottom), 
    //    1.0f, 5000.0f);
    //pD->SetTransform(D3DTS_PROJECTION, &m_projectionMatrix);
}

const D3DXMATRIX& ICamera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const D3DXMATRIX& ICamera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

TAG_CAMERA ICamera::GetTagCamera() const
{
    return m_tagCamera;
}
//-----------------------------------------------------------------
CameraFree::CameraFree()
    : ICamera(TAG_CAMERA::Default)
{
    m_position = D3DXVECTOR3(0.0f, 160.0f, -258.0f);
    //m_position = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
    D3DXQuaternionRotationYawPitchRoll(&m_quarernion, m_rotation.x, m_rotation.y, m_rotation.z);
}

CameraFree::~CameraFree()
{
}

void CameraFree::Reset()
{
}

void CameraFree::Update()
{

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
    //m_position = D3DXVECTOR3(FP_BASEPOSX, FP_BASEPOSY, -FP_DISTANCE);
    m_position = D3DXVECTOR3(FP_BASEPOSX, FP_BASEPOSY, FP_DISTANCE);
    m_rotation = D3DXVECTOR3( 0.0f, D3DX_PI, 0.0f);
    //70 Degrees FP sight
    m_fovY = D3DX_PI * (70.0f / 180.0f);

    D3DXQuaternionRotationYawPitchRoll(&m_quarernion, m_rotation.y, m_rotation.x, m_rotation.z);
}

void CameraFirstPerson::Update()
{
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
    m_rotation = D3DXVECTOR3( 0.0f, D3DX_PI, 0.0f);
    //80 Degrees TP sight
    m_fovY = D3DX_PI * (80.0f / 180.0f);
    
    bAltKeyPressed = false;
    
    //D3DXQuaternionRotationYawPitchRoll(&m_quarernion, m_rotation.y, m_rotation.x, m_rotation.z);
}

void CameraThirdPerson::Update()
{
    TargetTransform* pTarget = GetTarget();
    if (pTarget && pTarget->pRotForCameraTP)
    {
        const D3DXVECTOR3 rotForTP = *(pTarget->pRotForCameraTP);

        D3DXQuaternionRotationYawPitchRoll(&m_quarernion, rotForTP.y, rotForTP.x, rotForTP.z);
        m_quarernion *= pTarget->pTransform->GetRotation();
    }
}
//-----------------------------------------------------------------
//CameraTransform::CameraTransform()
//{
//
//}
//
//CameraTransform::~CameraTransform()
//{
//}
//
//void CameraTransform::Reset()
//{
//    CameraThirdPerson::Reset();
//    m_vel = 0.0f;
//
//}
//
//void CameraTransform::Update()
//{
//    CameraThirdPerson::Update();
//
//    //ALTkey�� ������ �ֺ��� �� �� �ִ� ���
//    
//    //Alt����� Icamera update�� �ѹ��� ���� ������ �̷��� �־����ϴ�.
//    //if (m_isALTbuttonStay)
//    //{
//    //}
//    //else//alt�� ������ �ʰ� �Ǹ� ĳ���Ͱ� �ٶ󺸰� �ִ� rotation ���� �ʱ�ȭ
//
//
//    //�����ϴ� �κ��� 3��Ī������ �ֱ⿡
//    //if (g_pKeyManager->IsOnceKeyDown(VK_RBUTTON))
//    //    g_pCameraManager->SetCurrentCamera(CameraState::KYUNCHAK);
//
//    //Ray ī�޶� ray�� �ڷ� ��Ƽ� ī�޶� m_distance�� �����ϴ� �κ�
//    //CamBackDistResizeing();
//    
//
//
//}

//-----------------------------------------------------------------

CameraKyunChak::CameraKyunChak()
    : CameraThirdPerson(TAG_CAMERA::KyunChak)
{

}

CameraKyunChak::~CameraKyunChak()
{
}

void CameraKyunChak::Reset()
{
    CameraThirdPerson::Reset();
    //m_distance = TP_DISTANCE;
    //m_basePosY = TP_BASEPOSY;
    //ThirdPersonCamera::Init();
}

void CameraKyunChak::Update()
{
    CameraThirdPerson::Update();


    const auto dt = Time()()->GetDeltaTime();
    const float factor = 5.0f;
    bool bR_buttonStay = Input()()->IsStayKeyDown(VK_RBUTTON);
    bool bR_buttonUp = Input()()->IsOnceKeyUp(VK_RBUTTON);

    //if (bR_buttonStay)//R_button�� ������ ������
    //{
    //    if (m_distance >= TP_DISTANCE - factor)//�������
    //    {
    //        m_vel += dt * factor;
    //        m_distance -= m_vel * dt;
    //        m_basePosY -= m_vel * dt * 0.5f;
    //    }
    //}
    //else//R_button�� ����������
    //{
    //    //bR_buttonUp true, distance�� �ణ ������/ �� (���� Ŭ���� ��� ��������)(�������� �Ѿ)
    //    if (bR_buttonUp&&m_distance >= TP_DISTANCE - 1.0f)
    //    {
    //        //!!! ������ �̰����� ĳ���Ͱ� ��� �ִ� �����ۿ� ����(2����,4���� no ���� ��) �ٲپ� �ִ� �ڵ带 ������ �Ѵ�.
    //        g_pCameraManager->SetCurrentCamera(CameraState::FIRSTPERSON);
    //        //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE2X);
    //        //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE4X);
    //    }
    //    else//�ƴѰ�� ��� �ٿ��ִٰ� ���� �ٴް� �Ǹ� TP�� �ٲ�
    //    {
    //        if (m_distance <= TP_DISTANCE - 0.15f)
    //        {
    //            m_vel -= dt * factor;
    //            m_distance += m_vel * dt;
    //            m_basePosY += m_vel * dt * 0.5f;
    //        }
    //        else
    //        {
    //            g_pCameraManager->SetCurrentCamera(CameraState::THIRDPERSON);
    //        }
    //    }

    //}


}








//-----------------------------------------------------------------
//const auto dt = Time()()->GetDeltaTime();
//
//m_vel += dt * 5.0f;
//m_distance -= m_vel * dt;
////m_fovY-= m_vel * dt*0.3f;
////D3DXMatrixPerspectiveFovLH(&m_matProj, m_fovY, m_aspect, 1, 1000);
////g_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
//if (m_distance <= TP_DISTANCE - 1.0f)
//    g_pCameraManager->SetCurrentCamera(CameraState::FIRSTPERSON);