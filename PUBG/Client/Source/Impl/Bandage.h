#pragma once
#include "IObject.h"

class EffectMeshRenderer;

class Bandage : public IObject
{
private:
    EffectMeshRenderer* pEMR;

public:
    Bandage();
    virtual ~Bandage();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};