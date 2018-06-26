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

    void Render();

    void SetEffectMesh(EffectMesh* pEffectMesh);
    void SetEffectMesh(const string& path, const string& xFilename);
};