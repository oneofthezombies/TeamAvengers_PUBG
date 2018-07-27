#pragma once
#include "TagClientOnly.h"
#include "Character.h" //ÈÆÈ¸Çü! struct °¡ ¾È¸ÔÇô¼­ ÀÌ·¸°Ô ³Ö¾ú´Âµ¥ ±¦Âú³ª¿ä?

#define TP_DISTANCE -30.0f
#define TP_BASEPOSX -50.0f
#define TP_BASEPOSY 45.0f


class ICamera : public MemoryAllocator
{
public:
    static const float VISUAL_RANGE;

private:
    const TAG_CAMERA    m_tagCamera;
          D3DXMATRIX    m_viewMatrix;
          D3DXMATRIX    m_projectionMatrix;

private:    //fustum Culling
    D3DXPLANE	        m_vecPlane[6];
    D3DXVECTOR3         m_vecWorld[8];
    D3DXVECTOR3	        m_vecProj[8];
    std::vector<WORD>   FRUSTUM_INDICES =
    {
        6, 2, 2, 3, 3, 7, 7, 6,
        2, 0, 0, 1, 1, 3,
        1, 5, 5, 7,
        0, 4, 4, 6,
        5, 4
    };

protected:  //Camera Position Rotation Fov

    D3DXMATRIX          m_worldMatrix;
    float               m_fovY;
    D3DXVECTOR3         m_eye;
    D3DXVECTOR3         m_look;

protected:
    //D3DXVECTOR3         m_vBulletDestination;
    vector<D3DXVECTOR3> drawRay;
    //bool                temp = false;  //ÃÑ½î¸é ³ª¿À´Â »¡°£»ö ray¸¦ À§ÇØ
protected:
    Character::Info*    GetTargetInfo();

public:
    ICamera(const TAG_CAMERA tag);
    virtual ~ICamera();

    virtual void Reset() = 0;
    virtual void Update() = 0;
    virtual void Render() {}    //for debug
    void CameraRender();
    void draw(const vector<D3DXVECTOR3>& vertices, const D3DXCOLOR& color);
    void drawIndices(const vector<WORD>& indices, const D3DXCOLOR& color);

    void UpdateViewProjMatrix();
    void UpdateFrustumCulling();
    bool IsObjectInsideFrustum(const D3DXVECTOR3 center,const float radius);

    void SetViewMatrix(D3DXMATRIX* viewMatrix) { m_viewMatrix = *viewMatrix; }
    void SetProjectionMatrix(D3DXMATRIX* projectionMatrix) { m_projectionMatrix = *projectionMatrix; }
    
    const D3DXMATRIX&  GetViewMatrix()       const;
    const D3DXMATRIX&  GetProjectionMatrix() const;
          TAG_CAMERA   GetTagCamera()        const;
    const D3DXVECTOR3& GetPosition()         const;
    D3DXVECTOR3 GetDirection();

    D3DXVECTOR4 GetFrustumArea();
    bool CalcPickedPosition(OUT D3DXVECTOR3 * vOut, WORD screenX, WORD screenY);
    Ray PickedRayDistancePosition(OUT D3DXVECTOR3* vOut, OUT float* distance, WORD screenX, WORD screenY);
};
 

class CameraFree : public ICamera
{
private:
    D3DXVECTOR3    m_position;
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
public:
    CameraThirdPerson(const TAG_CAMERA tag = TAG_CAMERA::Third_Person);
    virtual ~CameraThirdPerson();
    // Inherited via ICamera
    virtual void Reset() override;
    virtual void Update() override;
    virtual void Render() override;
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

class CameraOnGun : public ICamera
{

public:
    CameraOnGun();
    virtual ~CameraOnGun();
    // Inherited via ICamera
    virtual void Reset() override;
    virtual void Update() override;
    virtual void Render() override;
};

class CameraLobby : public ICamera
{
private:
    D3DXVECTOR3 m_rotation;
    D3DXVECTOR3 m_position;

public:
             CameraLobby();
    virtual ~CameraLobby();

    virtual void Reset() override;
    virtual void Update() override;
};