#pragma once
#include "IObject.h"

class SkinnedMeshController;

class Head : public IObject
{
private:
    SkinnedMeshController* pSkinnedMeshController;

public:
             Head();
    virtual ~Head();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};