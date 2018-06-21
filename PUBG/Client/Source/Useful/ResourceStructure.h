#pragma once

#ifdef OOTZ_DEBUG
class UIText;
#endif

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
};

struct Frame : public D3DXFRAME
{
    D3DXMATRIX CombinedTM;

//#ifdef OOTZ_DEBUG
//    string NameAndPosition;
//    UIText* pUINameAndPosition;
//#endif

    Frame();
    virtual ~Frame();
};

struct MeshContainer : public D3DXMESHCONTAINER
{
    EffectMesh*  pEffectMesh;
    LPD3DXMESH   pWorkMesh;
    D3DXMATRIX** ppBoneMatrixPtrs;
    D3DXMATRIX*  pBoneOffsetMatrices;
    D3DXMATRIX*  pFinalBoneMatrices;

    MeshContainer();
    virtual ~MeshContainer();
};

struct SkinnedMesh
{
    LPD3DXFRAME pRootFrame;
    LPD3DXANIMATIONCONTROLLER pAnimController;

    SkinnedMesh();
    ~SkinnedMesh();

    void SetupBoneMatrixPointers(LPD3DXFRAME pFrame);

private:
    void SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
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

    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};
