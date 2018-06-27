#pragma once
#include "TagClientOnly.h"

class ICamera : public MemoryAllocator
{
private:
    const TAG_CAMERA  m_tagCamera;
          D3DXMATRIX  m_viewMatrix;
          D3DXMATRIX  m_projectionMatrix;

protected:
    D3DXVECTOR3    m_position;
    D3DXQUATERNION m_rotation;
    float          m_fovY;

protected:
    Transform* getTarget();

public:
             ICamera(const TAG_CAMERA tag);
    virtual ~ICamera();

    virtual void Reset() = 0;
    virtual void Update() = 0;

    void UpdateViewProjMatrix();

    const D3DXMATRIX& GetViewMatrix()       const;
    const D3DXMATRIX& GetProjectionMatrix() const;
          TAG_CAMERA  GetTagCamera()        const;
};

class CameraFree : public ICamera
{
public:
             CameraFree();
    virtual ~CameraFree();

    virtual void Reset() override;
    virtual void Update() override;
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
