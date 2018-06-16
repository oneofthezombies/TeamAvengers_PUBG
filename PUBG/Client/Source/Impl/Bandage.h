#pragma once
#include "IObject.h"

class Renderer;

class Bandage : public IObject
{
private:
    Renderer* m_pRenderer;

public:
    Bandage();
    virtual ~Bandage();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};