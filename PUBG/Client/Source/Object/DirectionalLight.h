#pragma once
#include "MemoryAllocator.h"

class DirectionalLight : public MemoryAllocator
{
private:
    D3DXVECTOR3 m_direction;

public:
             DirectionalLight(
                 const D3DXVECTOR3& direction = 
                    D3DXVECTOR3(1.0f, -1.0f, 1.0f));
    virtual ~DirectionalLight();

    const D3DXVECTOR3& GetDirection();
};