#include "stdafx.h"
#include "Shader.h"

const char* Shader::World          = "World";
const char* Shader::View           = "View";
const char* Shader::Projection     = "Projection";
const char* Shader::bEmissiveColor = "bEmissiveColor";
const char* Shader::bLight         = "bLight";
const char* Shader::lightDirection = "lightDirection";
const char* Shader::DiffuseColor   = "DiffuseColor";
const char* Shader::SpecularPower  = "SpecularPower";
const char* Shader::SpecularColor  = "SpecularColor";
const char* Shader::EmissiveColor  = "EmissiveColor";

Shader::Manager::Manager()
    : Singleton<Shader::Manager>()
    , pCreateShadow(nullptr)
    , m_pShadowRenderTarget(nullptr)
    , m_pShadowDepthStencil(nullptr)
{
}

Shader::Manager::~Manager()
{
}

void Shader::Manager::addShadowSource(
    const D3DXMATRIX& world, 
    LPD3DXFRAME pFrame)
{
    if (!pFrame) return;

    LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
    while (pMeshContainer)
    {
        addShadowSource(world, pMeshContainer);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    addShadowSource(world, pFrame->pFrameSibling);
    addShadowSource(world, pFrame->pFrameFirstChild);
}

void Shader::Manager::addShadowSource(
    const D3DXMATRIX& world,
    LPD3DXMESHCONTAINER pMeshContainer)
{
    if (!pMeshContainer) return;

    MeshContainer* pMC = static_cast<MeshContainer*>(pMeshContainer);
    for (std::size_t i = 0; i < pMC->pEffectMesh->m_effectParams.size(); ++i)
        AddShadowSource(world, pMC->m_pWorkMesh, i);
}

void Shader::Manager::Init()
{
    Resource::XContainer* pXContainer = new Resource::XContainer;
    Resource::Async::CreateEffect(
        "./Resource/", 
        "create_shadow.fx", 
        pXContainer);
    Resource()()->AddResource(pXContainer);
    pCreateShadow = Resource()()->GetEffect(
        "./Resource/", 
        "create_shadow.fx");

    assert(pCreateShadow && "Shader::Manager::Init(), fx is null.");

    D3DXCreateTexture(
        Device()(), 
        SHADOWMAP_DIMENSION, 
        SHADOWMAP_DIMENSION, 
        1, 
        D3DUSAGE_RENDERTARGET, 
        D3DFMT_R32F, 
        D3DPOOL_DEFAULT, 
        &m_pShadowRenderTarget);

    Device()()->CreateDepthStencilSurface(
        SHADOWMAP_DIMENSION, 
        SHADOWMAP_DIMENSION, 
        D3DFMT_D24X8, 
        D3DMULTISAMPLE_NONE, 
        0, 
        true, 
        &m_pShadowDepthStencil, 
        nullptr);
}

void Shader::Manager::Destroy()
{
    SAFE_RELEASE(m_pShadowRenderTarget);
    SAFE_RELEASE(m_pShadowDepthStencil);
}

void Shader::Manager::AddShadowSource(
    const D3DXMATRIX& world, 
    SkinnedMesh* pSkinnedMesh)
{
    if (!pSkinnedMesh) return;

    addShadowSource(world, pSkinnedMesh->m_pRootFrame);
    addShadowSource(world, pSkinnedMesh->m_pSubRootFrame);
}

void Shader::Manager::AddShadowSource(
    const D3DXMATRIX& world, 
    EffectMesh* pEffectMesh)
{
    if (!pEffectMesh) return;

    for (std::size_t i = 0; i < pEffectMesh->m_effectParams.size(); ++i)
        AddShadowSource(world, pEffectMesh->m_pMesh, i);
}

void Shader::Manager::AddShadowSource(
    const D3DXMATRIX& world, 
          LPD3DXMESH pMesh, 
    const DWORD attribID)
{
    assert(pMesh && "Shader::Manager::AddVisible(), mesh is null.");

    m_shadowSources.emplace_back(std::make_tuple(world, pMesh, attribID));
}

void Shader::Manager::ClearShadowSources()
{
    m_shadowSources.resize(0);
}

void Shader::Manager::CreateShadowMap()
{
    LPDIRECT3DSURFACE9 pHWRenderTarget = nullptr;
    LPDIRECT3DSURFACE9 pHWDepthStencil = nullptr;
    Device()()->GetRenderTarget(0, &pHWRenderTarget);
    Device()()->GetDepthStencilSurface(&pHWDepthStencil);

    LPDIRECT3DSURFACE9 pShadowSurface = nullptr;
    m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface);
    Device()()->SetRenderTarget(0, pShadowSurface);
    pShadowSurface->Release();

    Device()()->SetDepthStencilSurface(m_pShadowDepthStencil);

    Device()()->Clear(
        0, 
        0, 
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
        D3DCOLOR_XRGB(255, 255, 255), 
        1.0f, 
        0);

    const D3DXVECTOR3 v3lightPos(Light()()->GetPosition());
    const D3DXVECTOR4 lightPos(v3lightPos.x, v3lightPos.y, v3lightPos.z, 1.0f);
    pCreateShadow->SetVector("gWorldLightPos", &lightPos);

    Light()()->SetMatrices();

    pCreateShadow->SetMatrix("gLightViewMatrix", &Light()()->GetViewMatrix());
    pCreateShadow->SetMatrix(
        "gLightProjMatrix", 
        &Light()()->GetProjectionMatrix());

    UINT numPasses = 0;
    pCreateShadow->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        pCreateShadow->BeginPass(i);

        for (auto& wmi : m_shadowSources)
        {
            D3DXMATRIX& world    = std::get<0>(wmi);
            LPD3DXMESH  pMesh    = std::get<1>(wmi);
            DWORD       attribID = std::get<2>(wmi);

            pCreateShadow->SetMatrix("gWorldMatrix", &world);
            pCreateShadow->CommitChanges();
            pMesh->DrawSubset(attribID);
        }

        pCreateShadow->EndPass();
    }
    pCreateShadow->End();

    ClearShadowSources();

    Device()()->SetRenderTarget(0, pHWRenderTarget);
    Device()()->SetDepthStencilSurface(pHWDepthStencil);

    pHWRenderTarget->Release();
    pHWDepthStencil->Release();
}

LPDIRECT3DTEXTURE9 Shader::Manager::GetShadowRenderTarget() const
{
    return m_pShadowRenderTarget;
}

Shader::Manager* Shader::operator()()
{
    return Shader::Manager::GetInstance();
}

void Shader::Draw(
    LPD3DXEFFECT pEffect, 
    D3DXHANDLE hParam, 
    LPD3DXMESH pMesh, 
    const DWORD attribID, 
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    assert(pEffect && pMesh && setGlobalVariable && "argument is null.");

    if (hParam)
        pEffect->ApplyParameterBlock(hParam);

    pEffect->SetMatrix(Shader::View, &CurrentCamera()()->GetViewMatrix());
    pEffect->SetMatrix(
        Shader::Projection,
        &CurrentCamera()()->GetProjectionMatrix());

    pEffect->SetTexture("ShadowMap_Tex", Shader()()->GetShadowRenderTarget());
    pEffect->SetMatrix("gLightViewMatrix", &Light()()->GetViewMatrix());
    pEffect->SetMatrix("gLightProjMatrix", &Light()()->GetProjectionMatrix());

    setGlobalVariable(pEffect);
    pEffect->CommitChanges();

    UINT numPasses = 0;
    pEffect->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        pEffect->BeginPass(i);
        pMesh->DrawSubset(attribID);
        pEffect->EndPass();
    }
    pEffect->End();
}

void Shader::Draw(
    const std::vector<EffectParam>& effectParams, 
    LPD3DXMESH pMesh, 
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable)
{
    assert(pMesh && setGlobalVariable && "Effectf::Draw(), argument is null.");

    for (std::size_t i = 0; i < effectParams.size(); ++i)
    {
        Draw(
            effectParams[i].pEffect, 
            effectParams[i].hParam, 
            pMesh, 
            i, 
            setGlobalVariable);
    }
}

void Shader::Draw(
    LPD3DXEFFECT pEffect, 
    D3DXHANDLE hParam,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable,
    const std::function<void()>& drawVertices)
{
    assert(pEffect && setGlobalVariable && drawVertices && 
        "Effectf::Draw(), argument is null.");

    if (hParam)
        pEffect->ApplyParameterBlock(hParam);

    pEffect->SetMatrix(Shader::View, &CurrentCamera()()->GetViewMatrix());
    pEffect->SetMatrix(
        Shader::Projection,
        &CurrentCamera()()->GetProjectionMatrix());

    pEffect->SetTexture("ShadowMap_Tex", Shader()()->GetShadowRenderTarget());
    pEffect->SetMatrix("gLightViewMatrix", &Light()()->GetViewMatrix());
    pEffect->SetMatrix("gLightProjMatrix", &Light()()->GetProjectionMatrix());

    setGlobalVariable(pEffect);
    pEffect->CommitChanges();

    UINT numPasses = 0;
    pEffect->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        pEffect->BeginPass(i);
        drawVertices();
        pEffect->EndPass();
    }
    pEffect->End();
}

void Shader::Draw(
    const std::vector<EffectParam>& effectParams,
    const std::function<void(LPD3DXEFFECT)>& setGlobalVariable,
    const std::function<void()>& drawVertices)
{
    assert(setGlobalVariable && drawVertices && 
        "Effectf::Draw(), argument is null.");

    for (std::size_t i = 0; i < effectParams.size(); ++i)
    {
        Draw(
            effectParams[i].pEffect, 
            effectParams[i].hParam, 
            setGlobalVariable, 
            drawVertices);
    }
}
