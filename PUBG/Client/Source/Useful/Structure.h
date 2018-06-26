#pragma once

#ifdef OOTZ_DEBUG
class UIText;
#endif

struct ResourceContainer;

struct VERTEX_PC
{
    D3DXVECTOR3 p;
    D3DCOLOR    c;

    VERTEX_PC();
    VERTEX_PC(const D3DXVECTOR3& p, const D3DCOLOR c);

    enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct VERTEX_RHWC
{
    D3DXVECTOR4 p;
    D3DCOLOR    c;

    VERTEX_RHWC();
    VERTEX_RHWC(const D3DXVECTOR4& p, const D3DCOLOR c);
    VERTEX_RHWC(const float x, const float y, const float z, 
        const float w, const D3DCOLOR c);

    enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
};

struct EffectParam
{
    string       Name;
    LPD3DXEFFECT pEffect;
    D3DXHANDLE   hParam;

    EffectParam();
    ~EffectParam();
};

struct EffectMesh
{
    LPD3DXMESH          pMesh;
    vector<EffectParam> EffectParams;

    EffectMesh();
    ~EffectMesh();

    void Render(const D3DXMATRIX& world, LPD3DXMESH pMesh);
};

struct Frame : public D3DXFRAME
{
    D3DXMATRIX CombinedTransformationMatrix;

//#ifdef OOTZ_DEBUG
//    string NameAndPosition;
//    UIText* pUINameAndPosition;
//#endif

    Frame();
};

struct MeshContainer : public D3DXMESHCONTAINER
{
    EffectMesh*  pEffectMesh;
    LPD3DXMESH   pWorkMesh;
    D3DXMATRIX** ppBoneMatrixPtrs;
    D3DXMATRIX*  pBoneOffsetMatrices;
    D3DXMATRIX*  pFinalBoneMatrices;

    MeshContainer();
};

struct SkinnedMesh
{
    LPD3DXFRAME               m_pRootFrame;
    LPD3DXANIMATIONCONTROLLER m_pAnimController;

    SkinnedMesh();
    ~SkinnedMesh();

    void Setup();

private:
    void setupBoneMatrixPointers(LPD3DXFRAME pFrame);
    void setupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
};

class AllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
    string m_path;
    string m_xFilename;

public:
    AllocateHierarchy();
    AllocateHierarchy(const string& path, const string& xFilename);
    ~AllocateHierarchy();

    STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
        LPD3DXFRAME *ppNewFrame) override;

    STDMETHOD(CreateMeshContainer)(THIS_
        LPCSTR Name,
        CONST D3DXMESHDATA *pMeshData,
        CONST D3DXMATERIAL *pMaterials,
        CONST D3DXEFFECTINSTANCE *pEffectInstances,
        DWORD NumMaterials,
        CONST DWORD *pAdjacency,
        LPD3DXSKININFO pSkinInfo,
        LPD3DXMESHCONTAINER *ppNewMeshContainer) override;

    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;

    STDMETHOD(DestroyMeshContainer)(
        THIS_ LPD3DXMESHCONTAINER pMeshContainerBase) override;
};

class AllocateHierarchyAsync : public ID3DXAllocateHierarchy
{
private:
    std::string        m_path;
    std::string        m_xFilename;
    ResourceContainer* pResourceContainer;

public:
    AllocateHierarchyAsync(const string& path, const string& xFilename, 
        ResourceContainer* pResourceContainer);
    ~AllocateHierarchyAsync();

    STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
        LPD3DXFRAME *ppNewFrame) override;

    STDMETHOD(CreateMeshContainer)(THIS_
        LPCSTR Name,
        CONST D3DXMESHDATA *pMeshData,
        CONST D3DXMATERIAL *pMaterials,
        CONST D3DXEFFECTINSTANCE *pEffectInstances,
        DWORD NumMaterials,
        CONST DWORD *pAdjacency,
        LPD3DXSKININFO pSkinInfo,
        LPD3DXMESHCONTAINER *ppNewMeshContainer) override;

    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameBase) override;

    STDMETHOD(DestroyMeshContainer)(
        THIS_ LPD3DXMESHCONTAINER pMeshContainerBase) override;
};