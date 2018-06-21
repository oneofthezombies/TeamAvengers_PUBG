#pragma once
#include "IObject.h"

class SkinnedMeshRenderer;
class Animator;
class MeshFilter;

class Kar98k : public IObject
{
private:
    SkinnedMeshRenderer* pSkinnedMeshRenderer;
    Animator*            pAnimator;
    MeshFilter*          pMeshFilter;

public:
    Kar98k();
    virtual ~Kar98k();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};