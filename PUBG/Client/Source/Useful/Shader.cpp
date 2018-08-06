#include "stdafx.h"
#include "Shader.h"

const float Shader::SHADOW_CULLING_DISTANCE = 5000.0f;

const char* Shader::World           = "World";
const char* Shader::View            = "View";
const char* Shader::Projection      = "Projection";
                                    
const char* Shader::bEmissiveColor  = "bEmissiveColor";
const char* Shader::bLight          = "bLight";
const char* Shader::bShadow         = "bShadow";
                                    
const char* Shader::LightPos        = "LightPos";
const char* Shader::LightView       = "LightView";
const char* Shader::LightProjection = "LightProjection";

const char* Shader::CameraPos       = "CameraPos";

const char* Shader::DiffuseColor    = "DiffuseColor";
const char* Shader::SpecularPower   = "SpecularPower";
const char* Shader::SpecularColor   = "SpecularColor";
const char* Shader::EmissiveColor   = "EmissiveColor";

const char* Shader::ShadowMap_Tex   = "ShadowMap_Tex";

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
    EffectMesh* pEffectMesh = pMC->pEffectMesh;

    for (std::size_t i = 0; i < pEffectMesh->m_effectParams.size(); ++i)
        AddShadowSource(world, pMC->m_pWorkMesh, static_cast<DWORD>(i));
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
        AddShadowSource(world, pEffectMesh->m_pMesh, static_cast<DWORD>(i));
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

    Light()()->SetMatrices();

    pCreateShadow->SetMatrix(Shader::LightView, &Light()()->GetViewMatrix());
    pCreateShadow->SetMatrix(
        Shader::LightProjection, 
        &Light()()->GetProjectionMatrix());

    UINT numPasses = 0;
    pCreateShadow->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        pCreateShadow->BeginPass(i);
        Debug << " shadow Num :" << m_shadowSources.size() << endl;
        for (auto& wmi : m_shadowSources)
        {
            D3DXMATRIX& world    = std::get<0>(wmi);
            LPD3DXMESH  pMesh    = std::get<1>(wmi);
            DWORD       attribID = std::get<2>(wmi);

            pCreateShadow->SetMatrix(Shader::World, &world);
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

    const D3DXVECTOR3 v3lightPos(Light()()->GetPosition());
    const D3DXVECTOR4 lightPos(v3lightPos.x, v3lightPos.y, v3lightPos.z, 1.0f);
    pEffect->SetVector(Shader::LightPos, &lightPos);

    pEffect->SetMatrix(Shader::LightView, &Light()()->GetViewMatrix());
    pEffect->SetMatrix(
        Shader::LightProjection, 
        &Light()()->GetProjectionMatrix());

    const D3DXVECTOR3 v3CameraPos(CurrentCamera()()->GetPosition());
    const D3DXVECTOR4 cameraPos(
        v3CameraPos.x, 
        v3CameraPos.y, 
        v3CameraPos.z, 
        1.0f);
    pEffect->SetVector(Shader::CameraPos, &cameraPos);

    pEffect->SetTexture(
        Shader::ShadowMap_Tex, 
        Shader()()->GetShadowRenderTarget());

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
            static_cast<DWORD>(i), 
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

    const D3DXVECTOR3 v3lightPos(Light()()->GetPosition());
    const D3DXVECTOR4 lightPos(v3lightPos.x, v3lightPos.y, v3lightPos.z, 1.0f);
    pEffect->SetVector(Shader::LightPos, &lightPos);

    pEffect->SetMatrix(Shader::LightView, &Light()()->GetViewMatrix());
    pEffect->SetMatrix(
        Shader::LightProjection,
        &Light()()->GetProjectionMatrix());

    const D3DXVECTOR3 v3CameraPos(CurrentCamera()()->GetPosition());
    const D3DXVECTOR4 cameraPos(
        v3CameraPos.x,
        v3CameraPos.y,
        v3CameraPos.z,
        1.0f);
    pEffect->SetVector(Shader::CameraPos, &cameraPos);

    pEffect->SetTexture(
        Shader::ShadowMap_Tex,
        Shader()()->GetShadowRenderTarget());

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
