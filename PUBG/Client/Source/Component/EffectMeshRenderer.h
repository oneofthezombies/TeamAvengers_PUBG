#pragma once
#include "ComponentTransform.h"

struct EffectMesh;

class EffectMeshRenderer : public Component
{
private:
    EffectMesh* pEffectMesh;
    LPD3DXMESH  m_pSphere;

public:
             EffectMeshRenderer(IObject* pOwner);
    virtual ~EffectMeshRenderer();

    void Render(const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

    void SetEffectMesh(EffectMesh* pEffectMesh);
    void SetEffectMesh(const TAG_RES_STATIC tag);
    EffectMesh* GetEffectMesh()
    {
        return pEffectMesh;
    }
};