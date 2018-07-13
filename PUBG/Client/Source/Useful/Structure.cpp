#include "stdafx.h"
#include "Structure.h"
#include "Light.h"

VERTEX_RHWC::VERTEX_RHWC()
    : p(0.0f, 0.0f, 0.0f, 0.0f)
    , c(D3DCOLOR_XRGB(0, 0, 0))
{
}

VERTEX_RHWC::VERTEX_RHWC(const D3DXVECTOR4& p, const D3DCOLOR c)
    : p(p)
    , c(c)
{
}

VERTEX_RHWC::VERTEX_RHWC(const float x, const float y, const float z, 
    const float w, const D3DCOLOR c)
    : p(x, y, z, w)
    , c(c)
{
}

EffectParam::EffectParam()
    : name("")
    , pEffect(nullptr)
    , hParam(nullptr)
{
}

EffectParam::~EffectParam()
{
}

EffectMesh::EffectMesh()
    : m_pMesh(nullptr)
{
}

EffectMesh::~EffectMesh()
{
    SAFE_RELEASE(m_pMesh);
    m_effectParams.resize(0);
}

Frame::Frame()
{
    memset(this, 0, sizeof Frame);
}

MeshContainer::MeshContainer()
{
    memset(this, 0, sizeof MeshContainer);
}

SkinnedMesh::SkinnedMesh()
    : m_pRootFrame(nullptr)
    , m_pSubRootFrame(nullptr)
    , pConnectFrame(nullptr)
    , m_pAnimController(nullptr)
    , m_pSubAnimController(nullptr)
    , m_index(std::numeric_limits<std::size_t>::max())
{
}

SkinnedMesh::~SkinnedMesh()
{
    Resource::Async::AllocateHierarchy ah;

    if (m_pRootFrame)
        D3DXFrameDestroy(m_pRootFrame, &ah);

    if (m_pSubRootFrame)
        D3DXFrameDestroy(m_pSubRootFrame, &ah);

    SAFE_RELEASE(m_pAnimController);
    SAFE_RELEASE(m_pSubAnimController);
}

void SkinnedMesh::Setup()
{
    setupBoneMatrixPointers(m_pRootFrame);
}

bool SkinnedMesh::Seperate(const string& name)
{
    if (m_pSubRootFrame) return false;

    seperate(m_pRootFrame, name);

    if (m_pSubRootFrame)
    {
        m_pAnimController->CloneAnimationController(
            m_pAnimController->GetMaxNumAnimationOutputs(),
            m_pAnimController->GetMaxNumAnimationSets(),
            m_pAnimController->GetMaxNumTracks(),
            m_pAnimController->GetMaxNumEvents(),
            &m_pSubAnimController);

        return true;
    }
    else
    {
        return false;
    }
}

void SkinnedMesh::setupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
    if (!pFrame) return;

    setupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
    setupBoneMatrixPointers(pFrame->pFrameSibling);
    setupBoneMatrixPointers(pFrame->pFrameFirstChild);
}

void SkinnedMesh::setupBoneMatrixPointersOnMesh(
    LPD3DXMESHCONTAINER pMeshContainerBase)
{
    if (!pMeshContainerBase) return;

    auto pMeshContainer = static_cast<MeshContainer*>(pMeshContainerBase);

    if (pMeshContainer->pSkinInfo)
    {
        auto numBones = pMeshContainer->pSkinInfo->GetNumBones();

        for (auto i = 0u; i < numBones; ++i)
        {
            auto pSearch = static_cast<Frame*>(D3DXFrameFind(
                m_pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(i)));

            pMeshContainer->m_ppBoneMatrixPtrs[i] = 
                &pSearch->CombinedTransformationMatrix;
        }
    }
}

void SkinnedMesh::seperate(LPD3DXFRAME pFrame, const string& name)
{
    if (m_pSubRootFrame) return;
    if (!pFrame) return;

    if (pFrame->pFrameFirstChild)
    {
        if (string(pFrame->pFrameFirstChild->Name) == name)
        {
            m_pSubRootFrame = pFrame->pFrameFirstChild;
            pConnectFrame = pFrame;
            pFrame->pFrameFirstChild = nullptr;
        }
    }

    if (pFrame->pFrameSibling)
    {
        if (string(pFrame->pFrameSibling->Name) == name)
        {
            m_pSubRootFrame = pFrame->pFrameSibling;
            pConnectFrame = pFrame;
            pFrame->pFrameSibling = nullptr;
        }
    }

    seperate(pFrame->pFrameSibling, name);
    seperate(pFrame->pFrameFirstChild, name);
}

//AllocateHierarchy::AllocateHierarchy()
//    : ID3DXAllocateHierarchy()
//    , m_path("")
//    , m_xFilename("")
//{
//}
//
//AllocateHierarchy::AllocateHierarchy(const string& path,
//    const string& xFilename)
//    : ID3DXAllocateHierarchy()
//    , m_path(path)
//    , m_xFilename(xFilename)
//{
//}
//
//AllocateHierarchy::~AllocateHierarchy()
//{
//}
//
//STDMETHODIMP AllocateHierarchy::CreateFrame(THIS_ LPCSTR Name,
//    LPD3DXFRAME* ppNewFrame)
//{
//    *ppNewFrame = nullptr;
//
//    Frame* pFrame = new Frame;
//    
//    if (!pFrame)
//        return E_OUTOFMEMORY;
//
//    if (Name)
//    {
//        const auto size = strlen(Name) + 1;
//        pFrame->Name = new char[size];
//        
//        if (!pFrame->Name)
//        {
//            delete pFrame;
//            return E_OUTOFMEMORY;
//        }
//
//        memcpy_s(pFrame->Name, size, Name, size);
//    }
//    else
//    {
//        pFrame->Name = nullptr;
//    }
//
//    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
//    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
//
//    pFrame->pMeshContainer = nullptr;
//    pFrame->pFrameSibling = nullptr;
//    pFrame->pFrameFirstChild = nullptr;
//
//    *ppNewFrame = pFrame;
//    pFrame = nullptr;
//
//    return S_OK;
//}
//
//STDMETHODIMP AllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name,
//    const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterials,
//    const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials,
//    const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
//    LPD3DXMESHCONTAINER* ppNewMeshContainer)
//{
//    assert(pMeshData->Type == D3DXMESHTYPE_MESH && 
//        "AllocateHierarchy::CreateMeshContainer(), \
//         Only MESHTYPE_MESH is possible.");
//
//    MeshContainer* pMeshContainer = new MeshContainer;
//
//    if (!pMeshContainer)
//        return E_OUTOFMEMORY;
//
//    if (Name)
//    {
//        const auto size = strlen(Name) + 1;
//        pMeshContainer->Name = new char[size];
//
//        if (!pMeshContainer->Name)
//        {
//            delete pMeshContainer;
//            return E_OUTOFMEMORY;
//        }
//        
//        memcpy_s(pMeshContainer->Name, size, Name, size);
//    }
//    else
//    {
//        pMeshContainer->Name = nullptr;
//    }
//
//    assert(pMeshContainer->Name && 
//        "AllocateHierarchy::CreateMeshContainer(), \
//         mesh container name is null.");
//
//    auto pEffectMesh = Resource()()->FindEffectMesh(
//        m_path, pMeshContainer->Name);
//
//    if (!pEffectMesh)
//    {
//        pEffectMesh = Resource()()->AddEffectMesh(
//            m_path, pMeshContainer->Name, pMeshData->pMesh, pEffectInstances, 
//            NumMaterials);
//
//        pEffectMesh->m_pMesh->AddRef();
//    }
//
//    pMeshContainer->pEffectMesh = pEffectMesh;
//    auto& pMesh = pMeshContainer->pEffectMesh->m_pMesh;
//
//    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE] = { 0 };
//    pMesh->GetDeclaration(decl);
//    pMesh->CloneMesh(pMesh->GetOptions(), decl, Device()(),
//        &pMeshContainer->m_pWorkMesh);
//
//    if (pSkinInfo)
//    {
//        pMeshContainer->pSkinInfo = pSkinInfo;
//        pSkinInfo->AddRef();
//
//        const auto numBones = pSkinInfo->GetNumBones();
//
//        pMeshContainer->m_pBoneOffsetMatrices = new D3DXMATRIX[numBones];
//        pMeshContainer->m_ppBoneMatrixPtrs = new D3DXMATRIX*[numBones];
//        pMeshContainer->m_pFinalBoneMatrices = new D3DXMATRIX[numBones];
//
//        auto& pBOMs = pMeshContainer->m_pBoneOffsetMatrices;
//        for (auto bi = 0u; bi < numBones; ++bi)
//            pBOMs[bi] = *pSkinInfo->GetBoneOffsetMatrix(bi);
//    }
//
//    *ppNewMeshContainer = pMeshContainer;
//    pMeshContainer = nullptr;
//
//    return S_OK;
//}
//
//STDMETHODIMP AllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
//{
//    SAFE_DELETE_ARRAY(pFrameToFree->Name);
//    SAFE_DELETE(pFrameToFree);
//    return S_OK;
//}
//
//STDMETHODIMP AllocateHierarchy::DestroyMeshContainer(
//    THIS_ LPD3DXMESHCONTAINER meshContainerBase)
//{
//    MeshContainer* pMeshContainer =
//        static_cast<MeshContainer*>(meshContainerBase);
//
//    if (!pMeshContainer) 
//        return E_FAIL;
//
//    SAFE_RELEASE(pMeshContainer->m_pWorkMesh);
//    SAFE_RELEASE(pMeshContainer->pSkinInfo);
//
//    SAFE_DELETE_ARRAY(pMeshContainer->Name);
//    SAFE_DELETE_ARRAY(pMeshContainer->m_pBoneOffsetMatrices);
//    SAFE_DELETE_ARRAY(pMeshContainer->m_ppBoneMatrixPtrs);
//    SAFE_DELETE_ARRAY(pMeshContainer->m_pFinalBoneMatrices);
//
//    SAFE_DELETE(pMeshContainer);
//    return S_OK;
//}

//AllocateHierarchyAsync::AllocateHierarchyAsync()
//    : ID3DXAllocateHierarchy()
//    , m_path("")
//    , m_xFilename("")
//    , pResourceContainer(nullptr)
//{
//}
//
//AllocateHierarchyAsync::AllocateHierarchyAsync(
//    const string & path, const string & xFilename, 
//    ResourceContainer* pResourceContainer)
//    : ID3DXAllocateHierarchy()
//    , m_path(path)
//    , m_xFilename(xFilename)
//    , pResourceContainer(pResourceContainer)
//{
//}
//
//AllocateHierarchyAsync::~AllocateHierarchyAsync()
//{
//}
//
//STDMETHODIMP AllocateHierarchyAsync::CreateFrame(
//    THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame)
//{
//    *ppNewFrame = nullptr;
//
//    LPSTR pName = nullptr;
//    if (Name)
//    {
//        const size_t size = strlen(Name) + 1;
//        pName = new char[size];
//        if (!pName)
//        {
//            return E_OUTOFMEMORY;
//        }
//
//        memcpy_s(pName, size, Name, size);
//    }
//
//    Frame* pFrame = new Frame;
//    if (!pFrame)
//    {
//        SAFE_DELETE_ARRAY(pName);
//        return E_OUTOFMEMORY;
//    }
//
//    pFrame->Name = pName;
//    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
//    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
//
//    *ppNewFrame = pFrame;
//    pFrame = nullptr;
//    return S_OK;
//}
//
//STDMETHODIMP AllocateHierarchyAsync::CreateMeshContainer(
//    THIS_ LPCSTR Name, const D3DXMESHDATA* pMeshData, 
//    const D3DXMATERIAL* pMaterials, const D3DXEFFECTINSTANCE* pEffectInstances, 
//    DWORD NumMaterials, const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
//    LPD3DXMESHCONTAINER* ppNewMeshContainer)
//{
//    if (pMeshData->Type != D3DXMESHTYPE_MESH)
//    {
//        return E_FAIL;
//    }
//
//    LPSTR pName = nullptr;
//    if (Name)
//    {
//        const size_t size = strlen(Name) + 1;
//        pName = new char[size];
//        if (!pName)
//        {
//            return E_OUTOFMEMORY;
//        }
//
//        memcpy_s(pName, size, Name, size);
//    }
//
//    const string meshContainerName = pName ? string(pName) : string("");
//
//    HRESULT hr = ResourceAsync::CreateEffectMesh(m_path, meshContainerName, 
//        pMeshData->pMesh, pEffectInstances, NumMaterials, pResourceContainer);
//
//    if (FAILED(hr))
//    {
//        SAFE_DELETE_ARRAY(pName);
//        return E_FAIL;
//    }
//
//    EffectMesh* pEffectMesh = 
//        pResourceContainer->m_effectMeshs[m_path + meshContainerName];
//    LPD3DXMESH pMesh = pEffectMesh->m_pMesh;
//    pMesh->AddRef();
//
//    LPD3DXMESH pWorkMesh = nullptr;
//    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE] = { 0 };
//    pMesh->GetDeclaration(decl);
//    pMesh->CloneMesh(pMesh->GetOptions(), decl, Device()(), &pWorkMesh);
//
//    D3DXMATRIX*  pBoneOffsetMatrices = nullptr;
//    D3DXMATRIX** ppBoneMatrixPtrs    = nullptr;
//    D3DXMATRIX*  pFinalBoneMatrices  = nullptr;
//    if (pSkinInfo)
//    {
//        pSkinInfo->AddRef();
//
//        const DWORD numBones = pSkinInfo->GetNumBones();
//        pBoneOffsetMatrices = new D3DXMATRIX [numBones];
//        ppBoneMatrixPtrs    = new D3DXMATRIX*[numBones];
//        pFinalBoneMatrices  = new D3DXMATRIX [numBones];
//
//        if (!pBoneOffsetMatrices || 
//            !ppBoneMatrixPtrs    || 
//            !pFinalBoneMatrices)
//        {
//            SAFE_DELETE_ARRAY(pName);
//            SAFE_DELETE_ARRAY(pBoneOffsetMatrices);
//            SAFE_DELETE_ARRAY(ppBoneMatrixPtrs);
//            SAFE_DELETE_ARRAY(pFinalBoneMatrices);
//            return E_OUTOFMEMORY;
//        }
//
//        for (DWORD i = 0; i < numBones; ++i)
//            pBoneOffsetMatrices[i] = *pSkinInfo->GetBoneOffsetMatrix(i);
//    }
//
//    MeshContainer* pMeshContainer = new MeshContainer;
//    if (!pMeshContainer)
//    {
//        SAFE_DELETE_ARRAY(pName);
//        SAFE_DELETE_ARRAY(pBoneOffsetMatrices);
//        SAFE_DELETE_ARRAY(ppBoneMatrixPtrs);
//        SAFE_DELETE_ARRAY(pFinalBoneMatrices);
//        return E_OUTOFMEMORY;
//    }
//
//    pMeshContainer->pEffectMesh = pEffectMesh;
//    pMeshContainer->m_pWorkMesh = pWorkMesh;
//    pMeshContainer->pSkinInfo   = pSkinInfo;
//
//    pMeshContainer->Name                  = pName;
//    pMeshContainer->m_pBoneOffsetMatrices = pBoneOffsetMatrices;
//    pMeshContainer->m_ppBoneMatrixPtrs    = ppBoneMatrixPtrs;
//    pMeshContainer->m_pFinalBoneMatrices  = pFinalBoneMatrices;
//
//    *ppNewMeshContainer = pMeshContainer;
//    pMeshContainer = nullptr;
//    return S_OK;
//}
//
//STDMETHODIMP AllocateHierarchyAsync::DestroyFrame(
//    THIS_ LPD3DXFRAME pFrameBase)
//{
//    if (!pFrameBase)
//    {
//        return E_FAIL;
//    }
//
//    SAFE_DELETE_ARRAY(pFrameBase->Name);
//    SAFE_DELETE(pFrameBase);
//    return S_OK;
//}
//
//STDMETHODIMP AllocateHierarchyAsync::DestroyMeshContainer(
//    THIS_ LPD3DXMESHCONTAINER pMeshContainerBase)
//{
//    MeshContainer* pMeshContainer =
//        static_cast<MeshContainer*>(pMeshContainerBase);
//
//    if (!pMeshContainer)
//    {
//        return E_FAIL;
//    }
//
//    SAFE_RELEASE(pMeshContainer->m_pWorkMesh);
//    SAFE_RELEASE(pMeshContainer->pSkinInfo);
//
//    SAFE_DELETE_ARRAY(pMeshContainer->Name);
//    SAFE_DELETE_ARRAY(pMeshContainer->m_pBoneOffsetMatrices);
//    SAFE_DELETE_ARRAY(pMeshContainer->m_ppBoneMatrixPtrs);
//    SAFE_DELETE_ARRAY(pMeshContainer->m_pFinalBoneMatrices);
//
//    SAFE_DELETE(pMeshContainer);
//    return S_OK;
//}
