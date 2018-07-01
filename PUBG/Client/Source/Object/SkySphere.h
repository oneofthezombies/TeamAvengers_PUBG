#pragma once
#include "IObject.h"

class EffectMeshRenderer;

class SkySphere : public IObject
{
private:
    EffectMeshRenderer* pEffectMeshRenderer;

public:
             SkySphere();
    virtual ~SkySphere();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};