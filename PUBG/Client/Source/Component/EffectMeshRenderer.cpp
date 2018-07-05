#include "stdafx.h"
#include "EffectMeshRenderer.h"

EffectMeshRenderer::EffectMeshRenderer(IObject* pOwner)
    : Component(pOwner)
    , m_pSphere(nullptr)
{
    
}

EffectMeshRenderer::~EffectMeshRenderer()
{
    SAFE_RELEASE(m_pSphere);
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

    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    //JOHN HAN
    //this is to draw sphere around the Items
    D3DXMATRIX mat = GetTransform()->GetTransformationMatrix();
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        m_pSphere,
        0,
        [this, &mat](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &mat);

        D3DXCOLOR Green(0.0f, 1.0f, 0.0f, 1.0f);
        pEffect->SetValue("Color", &Green, sizeof Green);
    });
    Device()()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void EffectMeshRenderer::SetEffectMesh(EffectMesh* pEffectMesh)
{
    assert(pEffectMesh && 
        "EffectMeshRenderer::SetEffectMesh(), effect mesh is null.");

    this->pEffectMesh = pEffectMesh;

    D3DXCreateSphere(Device()(), pEffectMesh->m_radius, 5, 5, &m_pSphere, nullptr);
}

void EffectMeshRenderer::SetEffectMesh(const TAG_RES_STATIC tag)
{
    SetEffectMesh(Resource()()->GetEffectMesh(tag));
}
