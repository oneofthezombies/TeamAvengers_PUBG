#pragma once
#include "IObject.h"

class SkinnedMeshRenderer;
class Animator;

class Kar98k : public IObject
{
private:
    SkinnedMeshRenderer* pRd;
    Animator*            pAt;

public:
    Kar98k();
    virtual ~Kar98k();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};