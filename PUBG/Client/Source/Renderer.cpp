#include "stdafx.h"
#include "Renderer.h"
#include "Animator.h"

Renderer::Renderer(IObject* pOwner, const TAG_RENDERER tag)
    : Component(pOwner)
    , m_tag(tag)
{
}

Renderer::~Renderer()
{
}

TAG_RENDERER Renderer::GetTag() const
{
    return m_tag;
}

void SkinnedMeshRenderer::DrawFrame(LPD3DXFRAME pFrame)
{
    if (!pFrame) return;

    auto pMeshContainer = pFrame->pMeshContainer;
    while (pMeshContainer)
    {
        DrawMeshContainer(pMeshContainer);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    DrawFrame(pFrame->pFrameSibling);
    DrawFrame(pFrame->pFrameFirstChild);
}

void SkinnedMeshRenderer::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainer)
{
    if (!pMeshContainer) return;
    if (!pMeshContainer->pSkinInfo) return;

    auto pMC = static_cast<MeshContainer*>(pMeshContainer);
    auto numBones = pMeshContainer->pSkinInfo->GetNumBones();
    for (auto i = 0u; i < numBones; ++i)
    {
        pMC->pFinalBoneMatrices[i] = pMC->pBoneOffsetMatrices[i] * 
            *pMC->ppBoneMatrixPtrs[i];
    }

    PBYTE pVerticesSrc = nullptr;
    pMC->pEffectMesh->pMesh->LockVertexBuffer(D3DLOCK_READONLY, 
        (LPVOID*)&pVerticesSrc);
    
    PBYTE pVerticesDest = nullptr;
    pMC->pWorkMesh->LockVertexBuffer(0, (LPVOID*)&pVerticesDest);

    pMC->pSkinInfo->UpdateSkinnedMesh(pMC->pFinalBoneMatrices, nullptr,
        pVerticesSrc, pVerticesDest);

    pMC->pEffectMesh->pMesh->UnlockVertexBuffer();
    pMC->pWorkMesh->UnlockVertexBuffer();

    for (size_t i = 0u; i < pMC->pEffectMesh->EffectParams.size(); ++i)
    {
        const auto& ep = pMC->pEffectMesh->EffectParams[i];
        ep.pEffect->ApplyParameterBlock(ep.hParam);

        // TODO :
        D3DXMATRIX s, t, world;
        D3DXMatrixScaling(&s, 0.1f, 0.1f, 0.1f);
        D3DXMatrixTranslation(&t, 0.0f, 0.0f, 10.0f);
        world = s * t;
        ep.pEffect->SetMatrix("World", &world);

        D3DXMATRIX view;
        ep.pEffect->SetMatrix("View", &g_pCurrentCamera->GetViewMatrix());

        D3DXMATRIX proj;
        ep.pEffect->SetMatrix("Projection", &g_pCurrentCamera->GetProjectionMatrix());

        //D3DXVECTOR3 lightDirection(1.0f, -1.0f, 1.0f);
        //D3DXVec3Normalize(&lightDirection, &lightDirection);
        //ep.pEffect->SetValue("lightDirection", &lightDirection, sizeof lightDirection);

        UINT numPasses;
        ep.pEffect->Begin(&numPasses, 0);
        for (UINT p = 0u; p < numPasses; ++p)
        {
            ep.pEffect->BeginPass(p);
            pMC->pWorkMesh->DrawSubset(i);
            ep.pEffect->EndPass();
        }
        ep.pEffect->End();
    }
}

SkinnedMeshRenderer::SkinnedMeshRenderer(IObject* pOwner)
    : Renderer(pOwner, TAG_RENDERER::SKINNED_MESH)
    , pAnimator(nullptr)
{
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
}

void SkinnedMeshRenderer::Render()
{
    if (!pAnimator)
        pAnimator = GetComponent<Animator>();

    if (pAnimator)
        DrawFrame(pAnimator->GetRootFrame());
}

EffectMeshRenderer::EffectMeshRenderer(IObject* pOwner)
    : Renderer(pOwner, TAG_RENDERER::EFFECT_MESH)
    , m_pEffectMesh(nullptr)
{
}

EffectMeshRenderer::~EffectMeshRenderer()
{
}

void EffectMeshRenderer::Render()
{
    for (size_t i = 0u; i < m_pEffectMesh->EffectParams.size(); ++i)
    {
        const auto& ep = m_pEffectMesh->EffectParams[i];
        ep.pEffect->ApplyParameterBlock(ep.hParam);

        // TODO :
        D3DXMATRIX s, t, world;
        D3DXMatrixScaling(&s, 0.1f, 0.1f, 0.1f);
        D3DXMatrixTranslation(&t, -1.0f, 0.0f, 10.0f);
        world = s * t;
        ep.pEffect->SetMatrix("World", &world);

        D3DXMATRIX view;
        ep.pEffect->SetMatrix("View", &g_pCurrentCamera->GetViewMatrix());

        D3DXMATRIX proj;
        ep.pEffect->SetMatrix("Projection", &g_pCurrentCamera->GetProjectionMatrix());

        //D3DXVECTOR3 lightDirection(1.0f, -1.0f, 1.0f);
        //D3DXVec3Normalize(&lightDirection, &lightDirection);
        //ep.pEffect->SetValue("lightDirection", &lightDirection, sizeof lightDirection);

        UINT numPasses;
        ep.pEffect->Begin(&numPasses, 0);
        for (UINT p = 0u; p < numPasses; ++p)
        {
            ep.pEffect->BeginPass(p);
            m_pEffectMesh->pMesh->DrawSubset(i);
            ep.pEffect->EndPass();
        }
        ep.pEffect->End();
    }
}

void EffectMeshRenderer::SetEffectMesh(EffectMesh* p)
{
    if (!p) return;

    m_pEffectMesh = p;
}
