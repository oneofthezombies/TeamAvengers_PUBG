#include "stdafx.h"
#include "ResourceStructure.h"

EffectParam::EffectParam()
    : pEffect(nullptr)
    , hParam(nullptr)
{
}

EffectParam::~EffectParam()
{
}

EffectMesh::EffectMesh()
    : pMesh(nullptr)
{
}

EffectMesh::~EffectMesh()
{
    SAFE_RELEASE(pMesh);
    EffectParams.resize(0);
}

Frame::Frame()
{
    memset(this, 0, sizeof Frame);
}

Frame::~Frame()
{
}

MeshContainer::MeshContainer()
{
    memset(this, 0, sizeof MeshContainer);
}

MeshContainer::~MeshContainer()
{
}

SkinnedMesh::SkinnedMesh()
    : pRootFrame(nullptr)
    , pAnimController(nullptr)
{
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
    if (pFrame->pMeshContainer)
        SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);

    if (pFrame->pFrameSibling)
        SetupBoneMatrixPointers(pFrame->pFrameSibling);

    if (pFrame->pFrameFirstChild)
        SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
}

void SkinnedMesh::SetupBoneMatrixPointersOnMesh(
    LPD3DXMESHCONTAINER pMeshContainerBase)
{
    auto pMeshContainer = static_cast<MeshContainer*>(pMeshContainerBase);

    if (pMeshContainer->pSkinInfo)
    {
        auto numBones = pMeshContainer->pSkinInfo->GetNumBones();

        for (auto i = 0u; i < numBones; ++i)
        {
            auto search = D3DXFrameFind(pRootFrame, 
                pMeshContainer->pSkinInfo->GetBoneName(i));
            auto pFrame = static_cast<Frame*>(search);
            pMeshContainer->ppBoneMatrixPtrs[i] = &pFrame->CombinedTM;
        }
    }
}

AllocateHierarchy::AllocateHierarchy()
    : ID3DXAllocateHierarchy()
    , m_path("")
    , m_xFilename("")
{
}

AllocateHierarchy::AllocateHierarchy(const string& path,
    const string& xFilename)
    : ID3DXAllocateHierarchy()
    , m_path(path)
    , m_xFilename(xFilename)
{
}

AllocateHierarchy::~AllocateHierarchy()
{
}

STDMETHODIMP AllocateHierarchy::CreateFrame(THIS_ LPCSTR Name,
    LPD3DXFRAME* ppNewFrame)
{
    Frame* pFrame = new Frame;

    if (Name)
    {
        const int size = strlen(Name) + 1;
        pFrame->Name = new char[size];
        strcpy_s(pFrame->Name, size, Name);
    }
    else
    {
        pFrame->Name = nullptr;
    }

    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTM);

    *ppNewFrame = pFrame;

    return S_OK;
}

STDMETHODIMP AllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name,
    const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterials,
    const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials,
    const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
    assert(pMeshData->Type == D3DXMESHTYPE_MESH &&
        "Only MESHTYPE_MESH is possible.");

    MeshContainer* pMeshContainer = new MeshContainer;

    if (Name)
    {
        const int size = strlen(Name) + 1;
        pMeshContainer->Name = new char[size];
        strcpy_s(pMeshContainer->Name, size, Name);
    }
    else
    {
        pMeshContainer->Name = nullptr;
    }

    EffectMesh* pEffectMesh = new EffectMesh;

    pEffectMesh->pMesh = pMeshData->pMesh;
    auto& pMesh = pEffectMesh->pMesh;
    pMesh->AddRef();
    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE] = { 0 };
    pMesh->GetDeclaration(decl);
    pMesh->CloneMesh(pMesh->GetOptions(), decl, g_pDevice, 
        &pMeshContainer->pWorkMesh);

    pEffectMesh->EffectParams.resize(NumMaterials);
    auto& pEI = pEffectInstances;

    for (DWORD i = 0u; i < NumMaterials; ++i)
    {
        LPD3DXEFFECT pEffect = g_pResourceManager->GetEffect(m_path,
            pEI[i].pEffectFilename);
        pEffectMesh->EffectParams[i].pEffect = pEffect;

        D3DXHANDLE hTech;
        pEffect->FindNextValidTechnique(nullptr, &hTech);
        pEffect->SetTechnique(hTech);

        pEffect->BeginParameterBlock();
        for (DWORD p = 0u; p < pEI[i].NumDefaults; ++p)
        {
            const auto pDefault = pEI[i].pDefaults[p];
            D3DXHANDLE handle = pEffect->GetParameterByName(nullptr,
                pDefault.pParamName);

            D3DXPARAMETER_DESC desc;
            if (handle)
            {
                pEffect->GetParameterDesc(handle, &desc);

                if (desc.Type == D3DXPT_BOOL ||
                    desc.Type == D3DXPT_INT ||
                    desc.Type == D3DXPT_FLOAT ||
                    desc.Type == D3DXPT_STRING)
                {
                    pEffect->SetValue(pDefault.pParamName, pDefault.pValue,
                        pDefault.NumBytes);
                }
                else if (desc.Type == D3DXPT_TEXTURE)
                {
                    pEffect->SetTexture(pDefault.pParamName,
                        g_pResourceManager->GetTexture(m_path,
                            string(static_cast<char*>(pDefault.pValue))));
                }
            }
        }
        pEffectMesh->EffectParams[i].hParam = pEffect->EndParameterBlock();
    }

    pMeshContainer->pEffectMesh = pEffectMesh;

    if (pSkinInfo)
    {
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        DWORD numBones = pSkinInfo->GetNumBones();
        pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[numBones];
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[numBones];
        pMeshContainer->pFinalBoneMatrices = new D3DXMATRIX[numBones];

        for (DWORD i = 0u; i < numBones; ++i)
            pMeshContainer->pBoneOffsetMatrices[i] = 
                *pSkinInfo->GetBoneOffsetMatrix(i);
    }

    *ppNewMeshContainer = pMeshContainer;

    return S_OK;
}

STDMETHODIMP AllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
    SAFE_DELETE_ARRAY(pFrameToFree->Name);
    SAFE_DELETE(pFrameToFree);

    return S_OK;
}

STDMETHODIMP AllocateHierarchy::DestroyMeshContainer(
    THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
    MeshContainer* pMeshContainer =
        static_cast<MeshContainer*>(pMeshContainerToFree);

    if (!pMeshContainer) return E_FAIL;

    SAFE_DELETE_ARRAY(pMeshContainer->Name);
    SAFE_RELEASE(pMeshContainer->pWorkMesh);

    SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
    SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
    SAFE_DELETE_ARRAY(pMeshContainer->pFinalBoneMatrices);

    SAFE_RELEASE(pMeshContainer->pSkinInfo);

    return S_OK;
}
