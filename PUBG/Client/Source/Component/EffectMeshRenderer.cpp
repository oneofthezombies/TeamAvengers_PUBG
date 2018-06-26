#include "stdafx.h"
#include "EffectMeshRenderer.h"

EffectMeshRenderer::EffectMeshRenderer(IObject* pOwner)
    : Component(pOwner)
{
}

EffectMeshRenderer::~EffectMeshRenderer()
{
}

void EffectMeshRenderer::Render()
{
    assert(pEffectMesh && 
        "EffectMeshController::Render(), effect mesh is null.");

    pEffectMesh->Render(
        GetTransform()->GetTransformationMatrix(), pEffectMesh->pMesh);
}

void EffectMeshRenderer::SetEffectMesh(EffectMesh* pEffectMesh)
{
    assert(pEffectMesh && 
        "EffectMeshRenderer::SetEffectMesh(), effect mesh is null.");

    this->pEffectMesh = pEffectMesh;
}

void EffectMeshRenderer::SetEffectMesh(const TAG_RES_STATIC tag)
{
    SetEffectMesh(Resource()()->GetEffectMesh(tag));
}

void EffectMeshRenderer::SetEffectMesh(const string& path, const string& xFilename)
{
    pEffectMesh = Resource()()->GetEffectMesh(path, xFilename);
}
