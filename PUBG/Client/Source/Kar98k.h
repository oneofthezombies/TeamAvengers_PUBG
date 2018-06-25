#pragma once
#include "IObject.h"

class SkinnedMeshController;

class Kar98k : public IObject
{
private:
    SkinnedMeshController* pSkinnedMeshController;
    LPD3DXMESH m_pSphereMesh;

public:
    Kar98k();
    virtual ~Kar98k();

    virtual void OnUpdate() override;
    virtual void OnRender() override;
};