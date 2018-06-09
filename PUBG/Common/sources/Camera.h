#pragma once
#include "BaseObject.h"

#define FP_DISTANCE 0.0f
#define FP_BASEPOSX 0.0f
#define FP_BASEPOSY 5.0f

#define TP_DISTANCE 10.0f
#define TP_BASEPOSX 2.0f
#define TP_BASEPOSY 8.0f

namespace CameraState
{
    enum CameraState
    {
        FIRSTPERSON = 1,
        THIRDPERSON = 3,
        TP2FP,
        FP2TP,
        KYUNCHAK,
        SCOPE2X,
        SCOPE4X
    };
}

class ICamera : public BaseObject
{
public:
    CameraState::CameraState     m_cameraState;
protected:
    D3DXVECTOR3		m_eye;
    D3DXVECTOR3		m_lookAt;
    D3DXVECTOR3		m_up;
    D3DXMATRIX	    m_matView;
    D3DXMATRIX	    m_matProj;
    D3DXVECTOR3		m_eyeBackRay;   //�ڷν�� ray�� ������
    D3DXVECTOR3		m_eyeDir;       //ī�޶� ���� ����


    float           m_basePosX;
    float			m_basePosY;
    float			m_distance;

    float			m_rotX;
    float			m_rotY;

    float           m_fovY;
    float           m_aspect;

    POINT			m_ptPrevMouse;

    bool            m_isALTbuttonStay;

    D3DXVECTOR3*    m_pTargetPos;//ĳ���� Position�� ������� ����
    D3DXVECTOR3*    m_pTargetRot;//ĳ���� Rotation�� ������� ����

    //vector<D3DXVECTOR3> m_vecProjVert;
    //vector<D3DXVECTOR3> m_vecWorldVert;
    //vector<D3DXPLANE>	m_vecSixPlane;

    ICamera();
public:
    virtual ~ICamera() {}
    virtual void Init();
    virtual void Update();

    //virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    //void SetTarget(D3DXVECTOR3* pTarget) { m_pTarget = pTarget; }
    
    //functions needed for backspace currling
    //void SetProjVert();
    //void ChangeToWorldVert();
    //void SetSixPlane();
    //bool CheckSphere(BoundingSphere* sphere);

    CameraState::CameraState GetState() const;
    D3DXVECTOR3 GetEyeDir()
    {
        return m_eyeDir;
    }
    D3DXVECTOR3 GetEyePos()
    {
        return m_eye;
    }
};

