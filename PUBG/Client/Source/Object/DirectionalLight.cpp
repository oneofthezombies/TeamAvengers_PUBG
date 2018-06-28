#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const D3DXVECTOR3& direction)
    : m_direction(direction)
{
}

DirectionalLight::~DirectionalLight()
{
}

const D3DXVECTOR3& DirectionalLight::GetDirection()
{
    return m_direction;
}
