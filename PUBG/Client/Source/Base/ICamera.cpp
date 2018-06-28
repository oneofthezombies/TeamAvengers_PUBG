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
//    //ALTkey를 누르면 주변을 볼 수 있는 기능
//    
//    //Alt기능을 Icamera update에 한번더 쓰기 때문에 이렇게 넣었습니다.
//    //if (m_isALTbuttonStay)
//    //{
//    //}
//    //else//alt를 누르지 않게 되면 캐릭터가 바라보고 있는 rotation 으로 초기화
//
//
//    //견착하는 부분은 3인칭에서만 있기에
//    //if (g_pKeyManager->IsOnceKeyDown(VK_RBUTTON))
//    //    g_pCameraManager->SetCurrentCamera(CameraState::KYUNCHAK);
//
//    //Ray 카메라 ray를 뒤로 쏘아서 카메라 m_distance를 조정하는 부분
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

    //if (bR_buttonStay)//R_button이 눌릴때 까지만
    //{
    //    if (m_distance >= TP_DISTANCE - factor)//견착모드
    //    {
    //        m_vel += dt * factor;
    //        m_distance -= m_vel * dt;
    //        m_basePosY -= m_vel * dt * 0.5f;
    //    }
    //}
    //else//R_button이 때어졌을때
    //{
    //    //bR_buttonUp true, distance가 약간 작을때/ 즉 (우측 클릭을 잠깐 눌렸을때)(조준으로 넘어감)
    //    if (bR_buttonUp&&m_distance >= TP_DISTANCE - 1.0f)
    //    {
    //        //!!! 앞으로 이곳에서 캐릭터가 들고 있는 아이템에 따라(2배율,4배율 no 배율 등) 바꾸어 주는 코드를 만들어야 한다.
    //        g_pCameraManager->SetCurrentCamera(CameraState::FIRSTPERSON);
    //        //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE2X);
    //        //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE4X);
    //    }
    //    else//아닌경우 계속 줄여주다가 끝에 다달게 되면 TP로 바꿈
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