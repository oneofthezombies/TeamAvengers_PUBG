#include "stdafx.h"
#include "Light.h"
#include "ComponentTransform.h"

Light::Manager::Manager()
    : Singleton<Light::Manager>()
    , m_positionInTargetSpace(Vector3::ZERO)
    , m_viewMatrix(Matrix::IDENTITY)
    , m_projectionMatrix(Matrix::IDENTITY)
    , pTarget(nullptr)
    , m_positionFactor(1000.0f, 0.0f, 1000.0f)
{
    m_projectVertices.resize(8);
    m_projectVertices[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
    m_projectVertices[1] = D3DXVECTOR3(-1.0f,  1.0f, 0.0f);
    m_projectVertices[2] = D3DXVECTOR3( 1.0f,  1.0f, 0.0f);
    m_projectVertices[3] = D3DXVECTOR3( 1.0f, -1.0f, 0.0f);
    m_projectVertices[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
    m_projectVertices[5] = D3DXVECTOR3(-1.0f,  1.0f, 1.0f);
    m_projectVertices[6] = D3DXVECTOR3( 1.0f,  1.0f, 1.0f);
    m_projectVertices[7] = D3DXVECTOR3( 1.0f, -1.0f, 1.0f);
}

Light::Manager::~Manager()
{
}

void Light::Manager::SetMatrices()
{
    D3DXVECTOR3 eye = m_positionInTargetSpace;
    D3DXVECTOR3 lookat = Vector3::ZERO;

    if (pTarget)
    {
        D3DXVECTOR3 pos = pTarget->GetPosition();
        pos += m_positionFactor;
        eye    += pos;
        lookat += pos;
    }

    D3DXMatrixLookAtLH(&m_viewMatrix, &eye, &lookat, &Vector3::UP);

    //// for perspective shadow
    //D3DXMatrixPerspectiveFovLH(
    //    &m_projectionMatrix, 
    //    D3DX_PI * 0.5f,
    //    1.0f, 
    //    1.0f, 
    //    ICamera::VISUAL_RANGE);

    // for directional light  
    D3DXMatrixOrthoLH(
        &m_projectionMatrix, 
        10000.0f, 
        10000.0f, 
        1.0f,
        ICamera::VISUAL_RANGE);
}

void Light::Manager::SetPositionInTargetSpace(const D3DXVECTOR3& pos)
{
    m_positionInTargetSpace = pos;
}

void Light::Manager::SetTarget(Transform* pTarget)
{
    this->pTarget = pTarget;
}

void Light::Manager::Render()
{
    if (!Collision()()->IsRender()) return;

    std::vector<D3DXVECTOR3> projectVertices = m_projectVertices;
    for (size_t i = 0; i < projectVertices.size(); i++)
    {
        D3DXVec3Unproject(
            &projectVertices[i], 
            &projectVertices[i], 
            nullptr, 
            &m_projectionMatrix, 
            &m_viewMatrix, 
            nullptr);
    }

    std::vector<D3DXVECTOR3> vertices(24);
    vertices[0] = projectVertices[0];
    vertices[1] = projectVertices[1];
    vertices[2] = projectVertices[1];
    vertices[3] = projectVertices[2];
    vertices[4] = projectVertices[2];
    vertices[5] = projectVertices[3];
    vertices[6] = projectVertices[3];
    vertices[7] = projectVertices[0];
    vertices[0 + 8] = projectVertices[0 + 4];
    vertices[1 + 8] = projectVertices[1 + 4];
    vertices[2 + 8] = projectVertices[1 + 4];
    vertices[3 + 8] = projectVertices[2 + 4];
    vertices[4 + 8] = projectVertices[2 + 4];
    vertices[5 + 8] = projectVertices[3 + 4];
    vertices[6 + 8] = projectVertices[3 + 4];
    vertices[7 + 8] = projectVertices[0 + 4];
    vertices[0 + 16] = projectVertices[0];
    vertices[1 + 16] = projectVertices[0 + 4];
    vertices[2 + 16] = projectVertices[1];
    vertices[3 + 16] = projectVertices[1 + 4];
    vertices[4 + 16] = projectVertices[2];
    vertices[5 + 16] = projectVertices[2 + 4];
    vertices[6 + 16] = projectVertices[3];
    vertices[7 + 16] = projectVertices[3 + 4];

    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        [](LPD3DXEFFECT pEffect) 
    {
        pEffect->SetMatrix(Shader::World, &Matrix::IDENTITY);

        D3DXVECTOR4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
        pEffect->SetVector("Color", &yellow);
    },
        [&vertices]() 
    {
        Device()()->DrawPrimitiveUP(
            D3DPT_LINELIST,
            static_cast<UINT>(vertices.size() / 2),
            vertices.data(),
            sizeof vertices.front());
    });

    D3DXMATRIX s, t, m;
    const float scaleFactor = 100.0f;
    D3DXMatrixScaling(&s, scaleFactor, scaleFactor, scaleFactor);
    const D3DXVECTOR3 pos = GetPosition();
    D3DXMatrixTranslation(&t, pos.x, pos.y, pos.z);
    m = s * t;

    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        Resource()()->GetBoundingSphereMesh(), 
        0,
        [&m](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &m);

        D3DXVECTOR4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
        pEffect->SetVector("Color", &yellow);
    });
}

const D3DXVECTOR3 Light::Manager::GetPosition() const
{
    D3DXVECTOR3 pos = m_positionInTargetSpace;

    if (pTarget)
    {
        const D3DXVECTOR3 targetPos = pTarget->GetPosition();
        pos += targetPos;
        pos += m_positionFactor;
    }
    return pos;
}

const D3DXMATRIX& Light::Manager::GetViewMatrix() const
{
    return m_viewMatrix;
}

const D3DXMATRIX& Light::Manager::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

Light::Manager* Light::operator()()
{
    return Light::Manager::GetInstance();
}
