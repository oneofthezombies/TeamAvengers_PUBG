#include "stdafx.h"
#include "EffectMeshRenderer.h"

EffectMeshRenderer::EffectMeshRenderer(IObject* pOwner)
    : Component(pOwner)
{
}

EffectMeshRenderer::~EffectMeshRenderer()
{
}

void EffectMeshRenderer::Render(
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    assert(pEffectMesh && setGlobalVariable &&
        "EffectMeshController::Render(), argument is null.");

    Shader::Draw(
        pEffectMesh->m_effectParams, 
        pEffectMesh->m_pMesh, 
        setGlobalVariable);
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
