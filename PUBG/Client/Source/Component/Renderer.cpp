#include "stdafx.h"
#include "Renderer.h"
#include "MeshFilterAndAnimator.h"
#include "Collider.h"
#include "UIText.h"

Renderer::Renderer(IObject* pOwner, const TAG_RENDERER tag)
    : Component(pOwner)
    , m_Tag(tag)
{
}

Renderer::~Renderer()
{
}

TAG_RENDERER Renderer::GetTag() const
{
    return m_Tag;
}

void SkinnedMeshRenderer::drawFrame(LPD3DXFRAME pFrameBase)
{
    if (!pFrameBase) return;

#ifdef OOTZ_DEBUG
    //PrintBoneNameAndPosition(pFrame);
#endif

    auto pMeshContainer = pFrameBase->pMeshContainer;
    while (pMeshContainer)
    {
        drawMeshContainer(pMeshContainer);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    drawFrame(pFrameBase->pFrameSibling);
    drawFrame(pFrameBase->pFrameFirstChild);
}

void SkinnedMeshRenderer::drawMeshContainer(
    LPD3DXMESHCONTAINER pMeshContainerBase)
{
    if (!pMeshContainerBase) return;
    if (!pMeshContainerBase->pSkinInfo) return;

#ifdef OOTZ_DEBUG
    //cout << "Mesh container name : ";
    //
    //if (pMeshContainer->Name)
    //    cout << string(pMeshContainer->Name) << '\n';
    //else
    //    cout << "NULL\n";
#endif

    auto pMeshContainer = static_cast<MeshContainer*>(pMeshContainerBase);
    auto numBones = pMeshContainerBase->pSkinInfo->GetNumBones();
    for (auto i = 0u; i < numBones; ++i)
    {
        pMeshContainer->pFinalBoneMatrices[i] 
            = pMeshContainer->pBoneOffsetMatrices[i] 
            * *pMeshContainer->ppBoneMatrixPtrs[i];
    }

    PBYTE pVerticesSrc = nullptr;
    pMeshContainer->pEffectMesh->pMesh->LockVertexBuffer(D3DLOCK_READONLY,
        (LPVOID*)&pVerticesSrc);
    
    PBYTE pVerticesDest = nullptr;
    pMeshContainer->pWorkMesh->LockVertexBuffer(0, (LPVOID*)&pVerticesDest);

    pMeshContainer->pSkinInfo->UpdateSkinnedMesh(
        pMeshContainer->pFinalBoneMatrices, nullptr, pVerticesSrc, 
        pVerticesDest);

    pMeshContainer->pEffectMesh->pMesh->UnlockVertexBuffer();
    pMeshContainer->pWorkMesh->UnlockVertexBuffer();

    const auto size = pMeshContainer->pEffectMesh->EffectParams.size();
    for (size_t i = 0u; i < size; ++i)
    {
        const auto& ep = pMeshContainer->pEffectMesh->EffectParams[i];

#ifdef OOTZ_DEBUG
        //cout << "Effect name : " << ep.Name << '\n';
#endif
        
        ep.pEffect->ApplyParameterBlock(ep.hParam);

        ep.pEffect->SetMatrix(
            "World", &GetOwnerTransform()->GetTransformationMatrix());

        D3DXMATRIX view;
        ep.pEffect->SetMatrix("View", &g_pCurrentCamera->GetViewMatrix());

        D3DXMATRIX proj;
        ep.pEffect->SetMatrix("Projection", 
            &g_pCurrentCamera->GetProjectionMatrix());

        //D3DXVECTOR3 lightDirection(1.0f, -1.0f, 1.0f);
        //D3DXVec3Normalize(&lightDirection, &lightDirection);
        //ep.pEffect->SetValue("lightDirection", &lightDirection, sizeof lightDirection);

        UINT numPasses;
        ep.pEffect->Begin(&numPasses, 0);
        for (UINT p = 0u; p < numPasses; ++p)
        {
            ep.pEffect->BeginPass(p);
            pMeshContainer->pWorkMesh->DrawSubset(i);
            ep.pEffect->EndPass();
        }
        ep.pEffect->End();
    }
}

void SkinnedMeshRenderer::printBoneNameAndPosition(LPD3DXFRAME pFrame)
{
    cout << "Frame name : ";

    if (pFrame->Name)
        cout << string(pFrame->Name) << '\n';
    else
        cout << "NULL\n";

    //auto pF = static_cast<Frame*>(pFrame);
    //
    //D3DXVECTOR3 worldPos(pF->CombinedTM._41, pF->CombinedTM._42, 
    //    pF->CombinedTM._43);
    //    
    //stringstream ss;
    //ss << (pFrame->Name ? string(pF->Name) : "NULL")
    //    << " " << worldPos;
    //    
    //pF->NameAndPosition = ss.str();
    //
    ////cout << pF->NameAndPosition << '\n';
    //
    //if (!pF->pUINameAndPosition)
    //{
    //    pF->pUINameAndPosition = new UIText(
    //        g_pFontManager->GetFont(TAG_FONT::DEFAULT),
    //        D3DXVECTOR2(700.0f, 25.0f), &pF->NameAndPosition,
    //        D3DCOLOR_XRGB(0, 255, 255), NULL);
    //    pF->pUINameAndPosition->SetDrawTextFormat(DT_LEFT | DT_VCENTER);
    //}
    //    
    //D3DVIEWPORT9 viewport;
    //g_pDevice->GetViewport(&viewport);
    //D3DXMATRIX proj, view;
    //g_pDevice->GetTransform(D3DTS_PROJECTION, &proj);
    //g_pDevice->GetTransform(D3DTS_VIEW, &view);
    //D3DXVECTOR3 viewportPos;
    //D3DXVec3Project(&viewportPos, &worldPos, &viewport, &proj, &view, NULL);
    //
    ////ss.str("");
    ////ss << viewportPos;
    ////cout << ss.str() << '\n';
    //
    //pF->pUINameAndPosition->SetPosition(viewportPos);
}

SkinnedMeshRenderer::SkinnedMeshRenderer(IObject* pOwner)
    : Renderer(pOwner, TAG_RENDERER::SKINNED_MESH)
    , pMeshFilter(nullptr)
{
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
}

void SkinnedMeshRenderer::Render()
{
    if (!pMeshFilter)
        pMeshFilter = GetComponent<MeshFilter>();

    assert(pMeshFilter && 
        "SkinnedMeshRenderer::Render() failed. mesh filter is null.");

    drawFrame(pMeshFilter->GetRootFrame());
}

EffectMeshRenderer::EffectMeshRenderer(IObject* pOwner)
    : Renderer(pOwner, TAG_RENDERER::EFFECT_MESH)
    , pEffectMesh(nullptr)
{
}

EffectMeshRenderer::~EffectMeshRenderer()
{
}

void EffectMeshRenderer::Render()
{
    for (size_t i = 0u; i < pEffectMesh->EffectParams.size(); ++i)
    {
        const auto& ep = pEffectMesh->EffectParams[i];
        ep.pEffect->ApplyParameterBlock(ep.hParam);

        ep.pEffect->SetMatrix(
            "World", &GetOwnerTransform()->GetTransformationMatrix());

        D3DXMATRIX view;
        ep.pEffect->SetMatrix("View", &g_pCurrentCamera->GetViewMatrix());

        D3DXMATRIX proj;
        ep.pEffect->SetMatrix("Projection", 
            &g_pCurrentCamera->GetProjectionMatrix());

        //D3DXVECTOR3 lightDirection(1.0f, -1.0f, 1.0f);
        //D3DXVec3Normalize(&lightDirection, &lightDirection);
        //ep.pEffect->SetValue("lightDirection", &lightDirection, sizeof lightDirection);

        UINT numPasses;
        ep.pEffect->Begin(&numPasses, 0);
        for (UINT p = 0u; p < numPasses; ++p)
        {
            ep.pEffect->BeginPass(p);
            pEffectMesh->pMesh->DrawSubset(i);
            ep.pEffect->EndPass();
        }
        ep.pEffect->End();
    }
}

void EffectMeshRenderer::SetEffectMesh(EffectMesh* p)
{
    assert(p && 
        "EffectMeshRenderer::SetEffectMesh() failed. effect mesh is null.");

    pEffectMesh = p;
}

void EffectMeshRenderer::SetEffectMesh(
    const string& path, const string& xFilename)
{
    SetEffectMesh(g_pResourceManager->GetEffectMesh(path, xFilename));
}

ColliderRenderer::ColliderRenderer(IObject* pOwner, const TAG_RENDERER tag)
    : Renderer(pOwner, tag)
    , m_Color(D3DCOLOR_XRGB(0, 255, 0))
{
}

ColliderRenderer::~ColliderRenderer()
{
}

void ColliderRenderer::Render()
{
}

void ColliderRenderer::SetColor(const D3DCOLOR color)
{
    m_Color = color;
}

BoxColliderRenderer::BoxColliderRenderer(IObject* pOwner)
    : ColliderRenderer(pOwner, TAG_RENDERER::BOX_COLLIDER)
    , pBoxCollider(nullptr)
{
}

BoxColliderRenderer::~BoxColliderRenderer()
{
}

void BoxColliderRenderer::Render()
{
    if (!pBoxCollider)
    {
        pBoxCollider = GetComponent<BoxCollider>();

        assert(pBoxCollider &&
            "BoxColliderRenderer::Render() failed. pBoxCollider is null.");

        auto extent = pBoxCollider->GetExtent();
        Init(-extent, extent);
    }

    vector<WORD> indices =
    {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7,
    };

    const auto pDevice = g_pDevice;
    pDevice->SetRenderState(D3DRS_LIGHTING, false);
    pDevice->SetTransform(D3DTS_WORLD, &pBoxCollider->GetTransform());
    pDevice->SetTexture(0, nullptr);
    pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, m_Vertices.size(),
        indices.size() / 2, indices.data(), D3DFMT_INDEX16, m_Vertices.data(), 
        sizeof VERTEX_PC);
}

void BoxColliderRenderer::Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
    m_Vertices.resize(8);
    m_Vertices[0] = VERTEX_PC(D3DXVECTOR3(min.x, min.y, min.z), m_Color);
    m_Vertices[1] = VERTEX_PC(D3DXVECTOR3(min.x, max.y, min.z), m_Color);
    m_Vertices[2] = VERTEX_PC(D3DXVECTOR3(max.x, max.y, min.z), m_Color);
    m_Vertices[3] = VERTEX_PC(D3DXVECTOR3(max.x, min.y, min.z), m_Color);
    m_Vertices[4] = VERTEX_PC(D3DXVECTOR3(min.x, min.y, max.z), m_Color);
    m_Vertices[5] = VERTEX_PC(D3DXVECTOR3(min.x, max.y, max.z), m_Color);
    m_Vertices[6] = VERTEX_PC(D3DXVECTOR3(max.x, max.y, max.z), m_Color);
    m_Vertices[7] = VERTEX_PC(D3DXVECTOR3(max.x, min.y, max.z), m_Color);
}
