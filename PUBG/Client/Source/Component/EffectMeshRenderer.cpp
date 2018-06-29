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

    //// TODO
    //D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
    //D3DXVec3TransformCoord(&vCenter, &pEffectMesh->m_center, &GetTransform()->GetTransformationMatrix());
    //Debug << "mesh center : " << pEffectMesh->m_center <<"center : " << vCenter << ", radius : " << pEffectMesh->m_radius << endl;
    //if (!CurrentCamera()()->IsObjectInsideFrustum(vCenter, pEffectMesh->m_radius));
    //    return;
    
    pEffectMesh->Render(GetTransform()->GetTransformationMatrix(), pEffectMesh->m_pMesh);
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
