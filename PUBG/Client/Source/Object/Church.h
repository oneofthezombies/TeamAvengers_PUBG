#pragma once
#include "IObject.h"

class EffectMeshRenderer;

class Church : public IObject
{
private:
    EffectMeshRenderer* pEffectMeshRenderer;

public:
             Church();
    virtual ~Church();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};