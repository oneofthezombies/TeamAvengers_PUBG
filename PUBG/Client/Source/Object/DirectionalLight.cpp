#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const D3DXVECTOR3& direction)
    : m_direction(direction)
{
    m_direction = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
}

DirectionalLight::~DirectionalLight()
{
}

const D3DXVECTOR3& DirectionalLight::GetDirection()
{
    return m_direction;
}
