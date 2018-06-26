#pragma once
#include "IObject.h"

class EffectMeshRenderer;
class BoxCollider;

class Bandage : public IObject
{
private:
    EffectMeshRenderer* pEffectMeshRenderer;
    BoxCollider*        pBoxCollider;

public:
    Bandage(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale);
    virtual ~Bandage();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};