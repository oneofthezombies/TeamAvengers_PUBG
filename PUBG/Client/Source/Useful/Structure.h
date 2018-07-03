#pragma once

#ifdef OOTZ_DEBUG
class UIText;
#endif

struct ResourceContainer;

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
    std::string  name;
    LPD3DXEFFECT pEffect;
    D3DXHANDLE   hParam;

    EffectParam();
    ~EffectParam();
};

struct EffectMesh
{
    LPD3DXMESH               m_pMesh;
    std::vector<EffectParam> m_effectParams;
    float                    m_radius;
    D3DXVECTOR3              m_center;

    EffectMesh();
    ~EffectMesh();
};

struct Frame : public D3DXFRAME
{
    D3DXMATRIX CombinedTransformationMatrix;

    Frame();
};

struct MeshContainer : public D3DXMESHCONTAINER
{
    EffectMesh*  pEffectMesh;
    LPD3DXMESH   m_pWorkMesh;
    D3DXMATRIX** m_ppBoneMatrixPtrs;
    D3DXMATRIX*  m_pBoneOffsetMatrices;
    D3DXMATRIX*  m_pFinalBoneMatrices;

    MeshContainer();
};

struct SkinnedMesh
{
    LPD3DXFRAME               m_pRootFrame;
    LPD3DXFRAME               m_pSubRootFrame;
    LPD3DXFRAME               pConnectFrame;

    LPD3DXANIMATIONCONTROLLER m_pAnimController;

    SkinnedMesh();
    ~SkinnedMesh();

private:
    void setupBoneMatrixPointers(LPD3DXFRAME pFrame);
    void setupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
    void seperate(LPD3DXFRAME pFrame, const string& name);

public:
    void Setup();
    bool Seperate(const string& name);
};

struct SkinnedMeshInstance
{
    SkinnedMesh* pSkinnedMesh;
    LPD3DXANIMATIONCONTROLLER m_pAnimController;
    LPD3DXANIMATIONCONTROLLER m_pSubAnimController;

    SkinnedMeshInstance();
    ~SkinnedMeshInstance();
};

///* do NOT use! this will be deleted soon.*/
//class AllocateHierarchy : public ID3DXAllocateHierarchy
//{
//private:
//    string m_path;
//    string m_xFilename;
//
//public:
//    AllocateHierarchy();
//    AllocateHierarchy(const string& path, const string& xFilename);
//    ~AllocateHierarchy();
//
//    STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
//        LPD3DXFRAME *ppNewFrame) override;
//
//    STDMETHOD(CreateMeshContainer)(THIS_
//        LPCSTR Name,
//        CONST D3DXMESHDATA *pMeshData,
//        CONST D3DXMATERIAL *pMaterials,
//        CONST D3DXEFFECTINSTANCE *pEffectInstances,
//        DWORD NumMaterials,
//        CONST DWORD *pAdjacency,
//        LPD3DXSKININFO pSkinInfo,
//        LPD3DXMESHCONTAINER *ppNewMeshContainer) override;
//
//    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
//
//    STDMETHOD(DestroyMeshContainer)(
//        THIS_ LPD3DXMESHCONTAINER pMeshContainerBase) override;
//};

class AllocateHierarchyAsync : public ID3DXAllocateHierarchy
{
private:
    std::string        m_path;
    std::string        m_xFilename;
    ResourceContainer* pResourceContainer;

public:
    AllocateHierarchyAsync();
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