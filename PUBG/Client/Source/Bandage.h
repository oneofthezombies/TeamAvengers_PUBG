#pragma once
#include "IObject.h"

class EffectMeshRenderer;
class BoxCollider;

class Bandage : public IObject
{
private:
    EffectMeshRenderer* pEffectMeshRenderer;
    BoxCollider* pBoxCollider;

public:
    Bandage();
    virtual ~Bandage();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};