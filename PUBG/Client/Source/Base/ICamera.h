#pragma once
#include "TagClientOnly.h"
#include "Character.h" //ÈÆÈ¸Çü! struct °¡ ¾È¸ÔÇô¼­ ÀÌ·¸°Ô ³Ö¾ú´Âµ¥ ±¦Âú³ª¿ä?

#define FP_DISTANCE -30.0f
#define FP_BASEPOSX 0.0f
#define FP_BASEPOSY 155.0f

#define TP_DISTANCE -30.0f
#define TP_BASEPOSX -50.0f
#define TP_BASEPOSY 45.0f


class ICamera : public MemoryAllocator
{
private:
    const TAG_CAMERA    m_tagCamera;
          D3DXMATRIX    m_worldMatrix;
          D3DXMATRIX    m_viewMatrix;
          D3DXMATRIX    m_projectionMatrix;

private:    //fustum Culling
    D3DXPLANE	        m_vecPlane[6];
    D3DXVECTOR3         m_vecWorld[8];
    D3DXVECTOR3	        m_vecProj[8];

protected:  //Camera Position Rotation Fov
    D3DXVECTOR3         m_position;
    D3DXQUATERNION      m_quarernion;
    float               m_fovY;

protected:
    //D3DXVECTOR3         m_vBulletDestination;
    vector<D3DXVECTOR3> drawRay;
    bool                temp = false;
protected:
    Character::Info*    GetTargetInfo();

public:
    ICamera(const TAG_CAMERA tag);
    virtual ~ICamera();

    virtual void Reset() = 0;
    virtual void Update() = 0;
    void CameraRender();
    void draw(const vector<D3DXVECTOR3>& vertices, const D3DXCOLOR& color);
    void drawIndices(const vector<WORD>& indices, const D3DXCOLOR& color);

    void UpdateViewProjMatrix();
    void UpdateFrustumCulling();
    bool IsObjectInsideFrustum(const D3DXVECTOR3 center,const float radius);

    void SetViewMatrix(D3DXMATRIX* viewMatrix) { m_viewMatrix = *viewMatrix; }
    void SetProjectionMatrix(D3DXMATRIX* projectionMatrix) { m_projectionMatrix = *projectionMatrix; }
    const D3DXMATRIX& GetViewMatrix()       const;
    const D3DXMATRIX& GetProjectionMatrix() const;
          TAG_CAMERA  GetTagCamera()        const;

    D3DXVECTOR4 GetFrustumArea();
    //Area CalculateFrustumAndArea(D3DXVECTOR3& destPos)
    //{
    //    UpdateFrustumCulling();
    //    D3DXVECTOR4 MinMax = GetFrustumArea();
    //    D3DXVECTOR3 min = D3DXVECTOR3(MinMax.x, 0, MinMax.y);
    //    D3DXVECTOR3 max = D3DXVECTOR3(MinMax.z, 0, MinMax.w);

    //    Area area;
    //    size_t index = CurrentScene()()->GetCellIndex(min);
    //    size_t index2 = CurrentScene()()->GetCellIndex(max);

    //    for (size_t i = 0; i < CellSpace::DIMENSION; i++)
    //    {
    //        

    //    }

    //    return area;
    //}

    bool CalcPickedPosition(OUT D3DXVECTOR3 * vOut, WORD screenX, WORD screenY);

};
 

class CameraFree : public ICamera
{
private:
    D3DXVECTOR3    m_rotation;
public:
    CameraFree();
    virtual ~CameraFree();

    virtual void Reset() override;
    virtual void Update() override;
};

class CameraFirstPerson : public ICamera
{
public:
    CameraFirstPerson(const TAG_CAMERA tag = TAG_CAMERA::First_Person);
    virtual ~CameraFirstPerson();

    // Inherited via ICamera
    virtual void Reset() override;
    virtual void Update() override;

};

class CameraThirdPerson : public ICamera
{
protected:
    bool bAltKeyPressed;
public:
    CameraThirdPerson(const TAG_CAMERA tag = TAG_CAMERA::Third_Person);
    virtual ~CameraThirdPerson();
    // Inherited via ICamera
    virtual void Reset() override;
    virtual void Update() override;
};


class CameraKyunChak : public CameraThirdPerson
{
private:
    float           m_vel;

public:
    CameraKyunChak();
    virtual ~CameraKyunChak();


    virtual void Reset() override;
    virtual void Update() override;
};

class Camera2xScope : public CameraFirstPerson
{
private:
    float           m_fovY_2x;
    float           m_deltaFovY;

    float           m_currTime;
    const float     m_totalTime;
public:
    Camera2xScope();
    virtual ~Camera2xScope();

    // Inherited via ThirdPersonCamera
    virtual void Reset() override;
    virtual void Update() override;

};
