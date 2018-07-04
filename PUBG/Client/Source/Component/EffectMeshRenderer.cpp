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


    //JOHN HAN
    //this is to draw sphere around the Items
    D3DXMATRIX mat;
    D3DXMatrixIdentity(&mat);
    Shader::Draw(
        Resource()()->GetEffect("./Resource/", "Color.fx"),
        nullptr,
        m_pSphere,
        0,
        [this, &mat](LPD3DXEFFECT pEffect)
    {
        pEffect->SetMatrix(Shader::World, &mat);

        D3DXCOLOR White(1.0f, 1.0f, 1.0f, 1.0f);
        pEffect->SetValue("Color", &White, sizeof White);
    });


}

void EffectMeshRenderer::SetEffectMesh(EffectMesh* pEffectMesh)
{
    assert(pEffectMesh && 
        "EffectMeshRenderer::SetEffectMesh(), effect mesh is null.");

    this->pEffectMesh = pEffectMesh;

    D3DXCreateSphere(Device()(), pEffectMesh->m_radius, 10, 10, &m_pSphere, nullptr);
}

void EffectMeshRenderer::SetEffectMesh(const TAG_RES_STATIC tag)
{
    SetEffectMesh(Resource()()->GetEffectMesh(tag));
}
