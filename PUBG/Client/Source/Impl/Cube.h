#pragma once

#include "IObject.h"

class Cube : public IObject
{
public:
    Cube();
    virtual ~Cube();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};