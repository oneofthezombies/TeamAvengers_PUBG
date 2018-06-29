#pragma once
#include "ComponentTransform.h"

struct EffectMesh;

class EffectMeshRenderer : public Component
{
private:
    EffectMesh* pEffectMesh;

public:
             EffectMeshRenderer(IObject* pOwner);
    virtual ~EffectMeshRenderer();

    void Render(const std::function<void(LPD3DXEFFECT)>& shaderGlobalSetup);

    void SetEffectMesh(EffectMesh* pEffectMesh);
    void SetEffectMesh(const TAG_RES_STATIC tag);

    /* do NOT use! this will be deleted soon.*/
    void SetEffectMesh(const string& path, const string& xFilename);
};