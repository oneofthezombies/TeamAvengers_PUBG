#pragma once
#include "IObject.h"

class EffectMeshRenderer;
class BoxCollider;
class BoxColliderRenderer;

class Bandage : public IObject
{
private:
    EffectMeshRenderer* pEffectMeshRenderer;
    BoxCollider* pBoxCollider;
    BoxColliderRenderer* pBoxColliderRenderer;

public:
    Bandage();
    virtual ~Bandage();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};