#pragma once

class Camera
{
private:
    D3DXMATRIX m_ViewMatrix;
    D3DXMATRIX m_ProjectionMatrix;

public:
    Camera();
    virtual ~Camera();

    const D3DXMATRIX& GetViewMatrix();
    const D3DXMATRIX& GetProjectionMatrix();
};

//#include "BaseObject.h"
//
//#define FP_DISTANCE 0.0f
//#define FP_BASEPOSX 0.0f
//#define FP_BASEPOSY 5.0f
//
//#define TP_DISTANCE 10.0f
//#define TP_BASEPOSX 2.0f
//#define TP_BASEPOSY 8.0f
//
//namespace CameraState
//{
//    enum CameraState
//    {
//        FIRSTPERSON = 1,
//        THIRDPERSON = 3,
//        TP2FP,
//        FP2TP,
//        KYUNCHAK,
//        SCOPE2X,
//        SCOPE4X
//    };
//}
//
//class ICamera : public BaseObject
//{
//public:
//    CameraState::CameraState     m_cameraState;
//protected:
//    D3DXVECTOR3		m_eye;
//    D3DXVECTOR3		m_lookAt;
//    D3DXVECTOR3		m_up;
//    D3DXMATRIX	    m_matView;
//    D3DXMATRIX	    m_matProj;
//    D3DXVECTOR3		m_eyeBackRay;   //뒤로쏘는 ray의 시작점
//    D3DXVECTOR3		m_eyeDir;       //카메라가 보는 방향
//
//
//    float           m_basePosX;
//    float			m_basePosY;
//    float			m_distance;
//
//    float			m_rotX;
//    float			m_rotY;
//
//    float           m_fovY;
//    float           m_aspect;
//
//    POINT			m_ptPrevMouse;
//
//    bool            m_isALTbuttonStay;
//
//    D3DXVECTOR3*    m_pTargetPos;//캐릭터 Position을 갖고오기 위함
//    D3DXVECTOR3*    m_pTargetRot;//캐릭터 Rotation을 갖고오기 위함
//
//    //vector<D3DXVECTOR3> m_vecProjVert;
//    //vector<D3DXVECTOR3> m_vecWorldVert;
//    //vector<D3DXPLANE>	m_vecSixPlane;
//
//    ICamera();
//public:
//    virtual ~ICamera() {}
//    virtual void Init();
//    virtual void Update();
//
//    //virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//    //void SetTarget(D3DXVECTOR3* pTarget) { m_pTarget = pTarget; }
//    
//    //functions needed for backspace currling
//    //void SetProjVert();
//    //void ChangeToWorldVert();
//    //void SetSixPlane();
//    //bool CheckSphere(BoundingSphere* sphere);
//
//    CameraState::CameraState GetState() const;
//    D3DXVECTOR3 GetEyeDir()
//    {
//        return m_eyeDir;
//    }
//    D3DXVECTOR3 GetEyePos()
//    {
//        return m_eye;
//    }
//};
//
