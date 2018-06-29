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
    /*, m_rotation(Vector3::ZERO)*/
    , m_fovY(D3DX_PI * 0.5f)
{
    //D3DXQuaternionRotationYawPitchRoll(&m_quarernion, m_rotation.x, m_rotation.y, m_rotation.z);
}

ICamera::~ICamera()
{
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
    D3DXQuaternionRotationYawPitchRoll(&m_quarernion,0.0f,0.0f,0.0f);
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

    bAltKeyPressed = false;
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
    //견착하는 부분은 3인칭에서만 있기에
    if (Input()()->IsOnceKeyDown(VK_RBUTTON))
        Camera()()->SetCurrentCamera(TAG_CAMERA::KyunChak);

}
//-----------------------------------------------------------------------
CameraKyunChak::CameraKyunChak()
    : CameraThirdPerson(TAG_CAMERA::KyunChak)
    , m_vel(0.0f)
{

}

CameraKyunChak::~CameraKyunChak()
{
}

void CameraKyunChak::Reset()
{
    CameraThirdPerson::Reset();
    m_position;
    m_fovY;
}

void CameraKyunChak::Update()
{
    CameraThirdPerson::Update();

    Debug << "m_position.x : " << m_position.x << "\n";
    Debug << "m_position.y : " << m_position.y << "\n";
    Debug << "m_position.z : " << m_position.z << "\n";
    Debug << "m_vel : " << m_vel << "\n";
    InputManager*  pInput = Input()();
    bool bR_buttonStay = pInput->IsStayKeyDown(VK_RBUTTON);
    bool bR_buttonUp = pInput->IsOnceKeyUp(VK_RBUTTON);


    const auto dt = Time()()->GetDeltaTime();
    const float factor = 100.0f;
    const float dtPow = pow(dt, 2); //dt^3

    if (bR_buttonStay)//R_button이 눌릴때 까지만
    {
        if (m_position.z > TP_DISTANCE * 0.5f)//견착모드
        {
            m_vel += dtPow * factor;
            m_position.z -= m_vel;
            m_position.y -= m_vel * 0.5f;
        }
    }
    else//R_button이 때어졌을때
    {
        //bR_buttonUp true, distance가 약간 작을때/ 즉 (우측 클릭을 잠깐 눌렸을때)(조준으로 넘어감)
        if (bR_buttonUp&&m_position.z >= TP_DISTANCE*0.9f)
        {
            //!!! 앞으로 이곳에서 캐릭터가 들고 있는 아이템에 따라(2배율,4배율 no 배율 등) 바꾸어 주는 코드를 만들어야 한다.
            Camera()()->SetCurrentCamera(TAG_CAMERA::First_Person);
            //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE2X);
            //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE4X);
        }
        else//아닌경우 계속 줄여주다가 끝에 다달게 되면 TP로 바꿈
        {
            if (m_position.z < TP_DISTANCE - (factor*0.018f))
            {
                m_vel -= dtPow * factor;
                m_position.z += m_vel;
                m_position.y += m_vel * 0.5f;
            }
            else
            {
                m_vel = 0.0f;
                Camera()()->SetCurrentCamera(TAG_CAMERA::Third_Person);
            }
        }

    }


}
