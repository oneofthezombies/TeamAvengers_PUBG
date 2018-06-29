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
    //�����ϴ� �κ��� 3��Ī������ �ֱ⿡
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

    if (bR_buttonStay)//R_button�� ������ ������
    {
        if (m_position.z > TP_DISTANCE * 0.5f)//�������
        {
            m_vel += dtPow * factor;
            m_position.z -= m_vel;
            m_position.y -= m_vel * 0.5f;
        }
    }
    else//R_button�� ����������
    {
        //bR_buttonUp true, distance�� �ణ ������/ �� (���� Ŭ���� ��� ��������)(�������� �Ѿ)
        if (bR_buttonUp&&m_position.z >= TP_DISTANCE*0.9f)
        {
            //!!! ������ �̰����� ĳ���Ͱ� ��� �ִ� �����ۿ� ����(2����,4���� no ���� ��) �ٲپ� �ִ� �ڵ带 ������ �Ѵ�.
            Camera()()->SetCurrentCamera(TAG_CAMERA::First_Person);
            //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE2X);
            //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE4X);
        }
        else//�ƴѰ�� ��� �ٿ��ִٰ� ���� �ٴް� �Ǹ� TP�� �ٲ�
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
