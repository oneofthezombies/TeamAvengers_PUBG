#include "stdafx.h"
#include "Light.h"

Light::Manager::Manager()
    : Singleton<Light::Manager>()
    , m_positionInTargetSpace(Vector3::ZERO)
    , pTargetPosition(nullptr)
    , m_viewMatrix(Matrix::IDENTITY)
    , m_projectionMatrix(Matrix::IDENTITY)
{
}

Light::Manager::~Manager()
{
}

void Light::Manager::SetMatrices()
{
    D3DXVECTOR3 eye = m_positionInTargetSpace;
    D3DXVECTOR3 lookat = Vector3::ZERO;

    if (pTargetPosition)
    {
        eye    += *pTargetPosition;
        lookat += *pTargetPosition;
    }

    D3DXMatrixLookAtLH(&m_viewMatrix, &eye, &lookat, &Vector3::UP);
    D3DXMatrixPerspectiveFovLH(
        &m_projectionMatrix, 
        D3DX_PI * 0.5f, 
        1.0f, 
        1.0f, 
        ICamera::VISUAL_RANGE);
}

void Light::Manager::SetPositionInTargetSpace(const D3DXVECTOR3& pos)
{
    m_positionInTargetSpace = pos;
}

void Light::Manager::SetTarget(D3DXVECTOR3* pTargetPosition)
{
    this->pTargetPosition = pTargetPosition;
}

const D3DXVECTOR3 Light::Manager::GetPosition() const
{
    D3DXVECTOR3 pos = m_positionInTargetSpace;

    if (pTargetPosition)
        pos += *pTargetPosition;

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
