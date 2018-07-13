#include "stdafx.h"
#include "Resource.h"
#include "ResourceInfo.h"

Resource::XContainer::XContainer()
    : m_filename("")
{
}

Resource::XContainer::~XContainer()
{
    for (auto& kv : m_effects)
    {
        auto& ef = kv.second;
        SAFE_RELEASE(ef);
    }

    for (auto& kv : m_textures)
    {
        auto& tx = kv.second;
        SAFE_RELEASE(tx);
    }

    for (auto& kv : m_effectMeshs)
    {
        auto& em = kv.second;
        SAFE_DELETE(em);
    }

    SAFE_DELETE(m_pSkinnedMesh.second);
}

Resource::XContainer* Resource::Async::OnLoadEffectMesh(
    const std::string path, 
    const std::string xFilename)
{
    const std::string fullPath(path + xFilename);
    LPD3DXBUFFER pEffectInstancesBuffer = nullptr;
    LPD3DXMESH   pMesh = nullptr;
    DWORD        numMaterials = 0;

    HRESULT hr = D3DXLoadMeshFromXA(
        fullPath.c_str(),
        D3DXMESH_MANAGED, 
        Device()(), 
        nullptr, 
        nullptr,
        &pEffectInstancesBuffer, 
        &numMaterials, 
        &pMesh);

    if (FAILED(hr))
    {
        return nullptr;
    }

    D3DXEFFECTINSTANCE* pEffectInstances =
        static_cast<D3DXEFFECTINSTANCE*>(
            pEffectInstancesBuffer->GetBufferPointer());

    XContainer* pXContainer = new XContainer;

    hr = CreateEffectMesh(
        path, 
        xFilename, 
        pMesh, 
        pEffectInstances,
        numMaterials, 
        pXContainer);

    if (FAILED(hr))
    {
        SAFE_DELETE(pXContainer);
        return nullptr;
    }

    pXContainer->m_filename = xFilename;
    return pXContainer;
}

Resource::XContainer* Resource::Async::OnLoadSkinnedMesh(
    const std::string path, 
    const std::string xFilename)
{
    const std::string fullPath(path + xFilename);

    XContainer* pXContainer = new XContainer;
    if (!pXContainer)
    {
        return nullptr;
    }

    pXContainer->m_pSkinnedMesh =
        std::make_pair(fullPath, new SkinnedMesh);
    SkinnedMesh* pSkinnedMesh = pXContainer->m_pSkinnedMesh.second;
    if (!pSkinnedMesh)
    {
        return nullptr;
    }

    AllocateHierarchy allocAsync(path, xFilename, pXContainer);

    HRESULT hr = D3DXLoadMeshHierarchyFromXA(
        fullPath.c_str(),
        D3DXMESH_MANAGED, 
        Device()(), 
        &allocAsync, 
        nullptr,
        &pSkinnedMesh->m_pRootFrame,
        &pSkinnedMesh->m_pAnimController);

    if (FAILED(hr))
    {
        return nullptr;
    }

    pXContainer->m_filename = xFilename;
    return pXContainer;
}

HRESULT Resource::Async::CreateEffectMesh(
    const std::string         path, 
    const std::string         name, 
          LPD3DXMESH          pMesh, 
    const D3DXEFFECTINSTANCE* pEffectInstances, 
    const DWORD               numEffectInstances, 
          XContainer*         OutXContainer)
{
    if (pMesh            == nullptr ||
        pEffectInstances == nullptr ||
        OutXContainer    == nullptr)
    {
        return E_FAIL;
    }

    const std::string key = path + name;
    const auto searchResult = OutXContainer->m_effectMeshs.find(key);
    if (searchResult != OutXContainer->m_effectMeshs.end())
    {
        return S_OK;
    }

    EffectMesh* pEffectMesh = new EffectMesh;
    OutXContainer->m_effectMeshs[key] = pEffectMesh;

    pEffectMesh->m_pMesh = pMesh;

    // TODO JOHN HAN 
    // create bounding sphere
    void* pData = nullptr;
    pEffectMesh->m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pData);
    D3DXComputeBoundingSphere(
        (D3DXVECTOR3*)pData,
        pEffectMesh->m_pMesh->GetNumVertices(),
        pEffectMesh->m_pMesh->GetNumBytesPerVertex(),
        &pEffectMesh->m_boundingSphere.center,
        &pEffectMesh->m_boundingSphere.radius);
    pEffectMesh->m_pMesh->UnlockVertexBuffer();
    // END TODO JOHN HAN

    for (DWORD i = 0; i < numEffectInstances; ++i)
    {
        const D3DXEFFECTINSTANCE& EI = pEffectInstances[i];

        if (!EI.pEffectFilename) continue;

        EffectParam effectParam;
        std::string effectParamName(EI.pEffectFilename);
        effectParam.name = effectParamName;

        HRESULT hr = CreateEffect(path, effectParamName, OutXContainer);

        if (FAILED(hr))
        {
            return E_FAIL;
        }

        effectParam.pEffect = OutXContainer->m_effects[path + effectParamName];

        D3DXHANDLE hTech = nullptr;
        LPD3DXEFFECT& pEffect = effectParam.pEffect;

        pEffect->FindNextValidTechnique(nullptr, &hTech);
        pEffect->SetTechnique(hTech);

        pEffect->BeginParameterBlock();
        for (DWORD di = 0; di < EI.NumDefaults; ++di)
        {
            _D3DXEFFECTDEFAULT& effectDefault = EI.pDefaults[di];

            D3DXHANDLE handle = pEffect->GetParameterByName(
                nullptr, effectDefault.pParamName);

            if (handle == nullptr) continue;

            D3DXPARAMETER_DESC desc;
            pEffect->GetParameterDesc(handle, &desc);

            switch (desc.Type)
            {
            case D3DXPT_BOOL:
            case D3DXPT_INT:
            case D3DXPT_FLOAT:
            case D3DXPT_STRING:
                {
                    pEffect->SetValue(effectDefault.pParamName,
                        effectDefault.pValue, effectDefault.NumBytes);
                }
                break;
            case D3DXPT_TEXTURE:
                {
                    const std::string textureFilename(
                        static_cast<char*>(effectDefault.pValue));

                    hr = CreateTexture(
                        path, 
                        textureFilename,
                        OutXContainer);

                    if (FAILED(hr))
                    {
                        return E_FAIL;
                    }

                    auto& textures = OutXContainer->m_textures;

                    pEffect->SetTexture(
                        effectDefault.pParamName,
                        textures[path + textureFilename]);
                }
                break;
            default:
                {
                }
                break;
            }
        }
        effectParam.hParam = pEffect->EndParameterBlock();
        pEffectMesh->m_effectParams.emplace_back(effectParam);
    }

    return S_OK;
}

HRESULT Resource::Async::CreateEffect(
    const std::string path, 
    const std::string filename, 
          XContainer* OutXContainer)
{
    if (OutXContainer == nullptr)
    {
        return E_FAIL;
    }

    const std::string key = path + filename;
    const auto searchResult = OutXContainer->m_effects.find(key);
    if (searchResult != OutXContainer->m_effects.end())
    {
        return S_OK;
    }

    LPD3DXEFFECT pEffect = nullptr;
    LPD3DXBUFFER pError = nullptr;
    DWORD flags = D3DXSHADER_DEBUG | D3DXFX_NOT_CLONEABLE;
    HRESULT hr = D3DXCreateEffectFromFileA(
        Device()(), 
        key.c_str(), 
        nullptr,
        nullptr, 
        flags, 
        nullptr, 
        &pEffect, 
        &pError);

    if (FAILED(hr))
    {
        if (pError)
        {
            std::string errorMessage(
                static_cast<char*>(pError->GetBufferPointer()),
                pError->GetBufferSize());

            MessageBoxA(nullptr, errorMessage.c_str(), nullptr, MB_OK);
        }

        return E_FAIL;
    }

    OutXContainer->m_effects[key] = pEffect;
    return S_OK;
}

HRESULT Resource::Async::CreateTexture(
    const std::string path, 
    const std::string filename,
          XContainer* OutXContainer)
{
    if (OutXContainer == nullptr)
    {
        return E_FAIL;
    }

    const std::string key(path + filename);
    const auto searchResult = OutXContainer->m_textures.find(key);
    if (searchResult != OutXContainer->m_textures.end())
    {
        return S_OK;
    }

    LPDIRECT3DTEXTURE9 pTexture = nullptr;
    HRESULT hr = D3DXCreateTextureFromFileExA(
        Device()(), 
        key.c_str(),
        D3DX_DEFAULT_NONPOW2, 
        D3DX_DEFAULT_NONPOW2, 
        D3DX_DEFAULT, 
        0,
        D3DFMT_UNKNOWN, 
        D3DPOOL_MANAGED, 
        D3DX_DEFAULT, 
        D3DX_DEFAULT, 
        0,
        nullptr, 
        nullptr, 
        &pTexture);

    if (FAILED(hr))
    {
        return E_FAIL;
    }

    OutXContainer->m_textures[key] = pTexture;
    return S_OK;
}

Resource::Async::AllocateHierarchy::AllocateHierarchy()
    : ID3DXAllocateHierarchy()
    , m_path("")
    , m_xFilename("")
    , pXContainer(nullptr)
{
}

Resource::Async::AllocateHierarchy::AllocateHierarchy(
    const std::string path, 
    const std::string xFilename,
          XContainer* OutXContainer)

    : ID3DXAllocateHierarchy()
    , m_path(path)
    , m_xFilename(xFilename)
    , pXContainer(OutXContainer)
{
}

STDMETHODIMP Resource::Async::AllocateHierarchy::CreateFrame(
    THIS_ LPCSTR       Name, 
          LPD3DXFRAME* ppNewFrame)
{
    *ppNewFrame = nullptr;

    LPSTR pName = nullptr;
    if (Name)
    {
        const std::size_t size = strlen(Name) + 1;
        pName = new char[size];
        if (!pName)
        {
            return E_OUTOFMEMORY;
        }

        memcpy_s(pName, size, Name, size);
    }

    Frame* pFrame = new Frame;
    if (!pFrame)
    {
        SAFE_DELETE_ARRAY(pName);
        return E_OUTOFMEMORY;
    }

    pFrame->Name = pName;
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

    *ppNewFrame = pFrame;
    pFrame = nullptr;
    return S_OK;
}

STDMETHODIMP Resource::Async::AllocateHierarchy::CreateMeshContainer(
    THIS_ LPCSTR               Name, 
    const D3DXMESHDATA*        pMeshData,
    const D3DXMATERIAL*        pMaterials, 
    const D3DXEFFECTINSTANCE*  pEffectInstances,
          DWORD                NumMaterials, 
    const DWORD*               pAdjacency, 
          LPD3DXSKININFO       pSkinInfo,
          LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
    if (pMeshData->Type != D3DXMESHTYPE_MESH)
    {
        return E_FAIL;
    }

    LPSTR pName = nullptr;
    if (Name)
    {
        const std::size_t size = strlen(Name) + 1;
        pName = new char[size];
        if (!pName)
        {
            return E_OUTOFMEMORY;
        }

        memcpy_s(pName, size, Name, size);
    }

    const std::string meshContainerName = 
        pName ? std::string(pName) : std::string("");

    HRESULT hr = CreateEffectMesh(
        m_path, 
        meshContainerName,
        pMeshData->pMesh, 
        pEffectInstances, 
        NumMaterials, 
        pXContainer);

    if (FAILED(hr))
    {
        SAFE_DELETE_ARRAY(pName);
        return E_FAIL;
    }

    EffectMesh* pEffectMesh =
        pXContainer->m_effectMeshs[m_path + meshContainerName];
    LPD3DXMESH pMesh = pEffectMesh->m_pMesh;
    pMesh->AddRef();

    LPD3DXMESH pWorkMesh = nullptr;
    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE] = { 0 };
    pMesh->GetDeclaration(decl);
    pMesh->CloneMesh(pMesh->GetOptions(), decl, Device()(), &pWorkMesh);

    D3DXMATRIX*  pBoneOffsetMatrices = nullptr;
    D3DXMATRIX** ppBoneMatrixPtrs = nullptr;
    D3DXMATRIX*  pFinalBoneMatrices = nullptr;
    if (pSkinInfo)
    {
        pSkinInfo->AddRef();

        const DWORD numBones = pSkinInfo->GetNumBones();
        pBoneOffsetMatrices = new D3DXMATRIX [numBones];
        ppBoneMatrixPtrs    = new D3DXMATRIX*[numBones];
        pFinalBoneMatrices  = new D3DXMATRIX [numBones];

        if (!pBoneOffsetMatrices ||
            !ppBoneMatrixPtrs ||
            !pFinalBoneMatrices)
        {
            SAFE_DELETE_ARRAY(pName);
            SAFE_DELETE_ARRAY(pBoneOffsetMatrices);
            SAFE_DELETE_ARRAY(ppBoneMatrixPtrs);
            SAFE_DELETE_ARRAY(pFinalBoneMatrices);
            return E_OUTOFMEMORY;
        }

        for (DWORD i = 0; i < numBones; ++i)
            pBoneOffsetMatrices[i] = *pSkinInfo->GetBoneOffsetMatrix(i);
    }

    MeshContainer* pMeshContainer = new MeshContainer;
    if (!pMeshContainer)
    {
        SAFE_DELETE_ARRAY(pName);
        SAFE_DELETE_ARRAY(pBoneOffsetMatrices);
        SAFE_DELETE_ARRAY(ppBoneMatrixPtrs);
        SAFE_DELETE_ARRAY(pFinalBoneMatrices);
        return E_OUTOFMEMORY;
    }

    pMeshContainer->pEffectMesh = pEffectMesh;
    pMeshContainer->m_pWorkMesh = pWorkMesh;
    pMeshContainer->pSkinInfo = pSkinInfo;

    pMeshContainer->Name = pName;
    pMeshContainer->m_pBoneOffsetMatrices = pBoneOffsetMatrices;
    pMeshContainer->m_ppBoneMatrixPtrs = ppBoneMatrixPtrs;
    pMeshContainer->m_pFinalBoneMatrices = pFinalBoneMatrices;

    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = nullptr;
    return S_OK;
}

STDMETHODIMP Resource::Async::AllocateHierarchy::DestroyFrame(
    THIS_ LPD3DXFRAME pFrameBase)
{
    if (!pFrameBase)
    {
        return E_FAIL;
    }

    SAFE_DELETE_ARRAY(pFrameBase->Name);
    SAFE_DELETE(pFrameBase);
    return S_OK;
}

STDMETHODIMP Resource::Async::AllocateHierarchy::DestroyMeshContainer(
    THIS_ LPD3DXMESHCONTAINER pMeshContainerBase)
{
    MeshContainer* pMeshContainer =
        static_cast<MeshContainer*>(pMeshContainerBase);

    if (!pMeshContainer)
    {
        return E_FAIL;
    }

    SAFE_RELEASE(pMeshContainer->m_pWorkMesh);
    SAFE_RELEASE(pMeshContainer->pSkinInfo);

    SAFE_DELETE_ARRAY(pMeshContainer->Name);
    SAFE_DELETE_ARRAY(pMeshContainer->m_pBoneOffsetMatrices);
    SAFE_DELETE_ARRAY(pMeshContainer->m_ppBoneMatrixPtrs);
    SAFE_DELETE_ARRAY(pMeshContainer->m_pFinalBoneMatrices);

    SAFE_DELETE(pMeshContainer);
    return S_OK;
}

Resource::Manager::Manager()
    : Singleton<Resource::Manager>()
    , m_pEffectPool(nullptr)
    , m_pBoundingSphereMesh(nullptr)
{
}

void Resource::Manager::Init()
{
    HRESULT hr = D3DXCreateEffectPool(&m_pEffectPool);

    assert(!FAILED(hr) && 
        "Resource::Manager::Init(), \
         D3DXCreateEffectPool() failed.");

    // create color line shader
    XContainer* pXContainer = new XContainer;
    hr = Async::CreateEffect("./Resource/", "Color.fx", pXContainer);
    AddResource(pXContainer);

    // create boundingSphere mesh
    D3DXCreateSphere(Device()(), 1.0f, 5, 5, &m_pBoundingSphereMesh, nullptr);

    // create boundingBox vertices
    D3DXVECTOR3 min(-Vector3::ONE);
    D3DXVECTOR3 max(Vector3::ONE);

    m_boundingBoxVertices.resize(8);
    m_boundingBoxVertices[0] = D3DXVECTOR3(min.x, min.y, min.z);
    m_boundingBoxVertices[1] = D3DXVECTOR3(min.x, max.y, min.z);
    m_boundingBoxVertices[2] = D3DXVECTOR3(max.x, max.y, min.z);
    m_boundingBoxVertices[3] = D3DXVECTOR3(max.x, min.y, min.z);
    m_boundingBoxVertices[4] = D3DXVECTOR3(min.x, min.y, max.z);
    m_boundingBoxVertices[5] = D3DXVECTOR3(min.x, max.y, max.z);
    m_boundingBoxVertices[6] = D3DXVECTOR3(max.x, max.y, max.z);
    m_boundingBoxVertices[7] = D3DXVECTOR3(max.x, min.y, max.z);

    m_boundingBoxIndices =
    {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7,
    };
}

void Resource::Manager::Destroy()
{
    SAFE_RELEASE(m_pEffectPool);
    SAFE_RELEASE(m_pBoundingSphereMesh);

    for (auto e : m_effects)
        SAFE_RELEASE(e.second);

    for (auto t : m_textures)
        SAFE_RELEASE(t.second);

    for (auto f : m_fonts)
        SAFE_RELEASE(f.second);

    for (auto em : m_effectMeshs)
        SAFE_DELETE(em.second);

    for (auto sm : m_skinnedMeshs)
        SAFE_DELETE(sm.second);

    //RemoveFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
}

void Resource::Manager::AddResource(XContainer* pXContainer)
{
    assert(pXContainer &&
        "Resource::Manager::AddResource(), argument is null.");

    for (auto& kv : pXContainer->m_effectMeshs)
    {
        const std::string key = kv.first;
        EffectMesh*& pEffectMesh = kv.second;

        const auto search = m_effectMeshs.find(key);
        if (search == m_effectMeshs.end())
        {
            m_effectMeshs[key] = pEffectMesh;
            pEffectMesh = nullptr;
        }
    }

    for (auto& kv : pXContainer->m_effects)
    {
        const std::string key = kv.first;
        LPD3DXEFFECT& pEffect = kv.second;

        const auto search = m_effects.find(key);
        if (search == m_effects.end())
        {
            m_effects[key] = pEffect;
            pEffect = nullptr;
        }
    }

    for (auto& kv : pXContainer->m_textures)
    {
        const std::string key = kv.first;
        LPDIRECT3DTEXTURE9& pTexture = kv.second;

        const auto search = m_textures.find(key);
        if (search == m_textures.end())
        {
            m_textures[key] = pTexture;
            pTexture = nullptr;
        }
    }

    if (pXContainer->m_pSkinnedMesh.second)
    {
        const std::string key = pXContainer->m_pSkinnedMesh.first;
        const auto search = m_skinnedMeshs.find(key);
        if (search == m_skinnedMeshs.end())
        {
            m_skinnedMeshs[key] = pXContainer->m_pSkinnedMesh.second;
            pXContainer->m_pSkinnedMesh.second = nullptr;

            m_skinnedMeshs[key]->Setup();
        }
    }

    SAFE_DELETE(pXContainer);
}

void Resource::Manager::AddTexture(const TAG_RES_STATIC tag, const D3DCOLOR colorKey)
{
    const auto p = ResourceInfo::GetUIPathFileName(tag);
    AddTexture(p.first, p.second, colorKey);
}

void Resource::Manager::AddTexture(const string& path, const string& xFilename, const D3DCOLOR colorKey)
{
    const string key(path + xFilename);

    LPDIRECT3DTEXTURE9 pTexture = nullptr;
    HRESULT hr = D3DXCreateTextureFromFileExA(
        Device()(),
        key.c_str(),
        D3DX_DEFAULT_NONPOW2,
        D3DX_DEFAULT_NONPOW2,
        D3DX_DEFAULT,
        0,
        D3DFMT_UNKNOWN,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        colorKey,
        nullptr,
        nullptr,
        &pTexture);

    assert(!FAILED(hr) && "Resource::Manager::AddTexture(), D3DXCreateTextureFromFileExA() failed.");

    m_textures[key] = pTexture;
}

LPD3DXFONT Resource::Manager::GetFont(const TAG_FONT tag)
{
    const auto search = m_fonts.find(tag);
    HRESULT hr = -1;
    if (search == m_fonts.end())
    {
        switch (tag)
        {
        case TAG_FONT::Default:
            {
                hr = D3DXCreateFontA(
                    Device()(), 
                    24, 
                    12, 
                    FW_NORMAL, 
                    1, 
                    false,
                    HANGEUL_CHARSET,
                    OUT_DEFAULT_PRECIS, 
                    DEFAULT_QUALITY,
                    FF_DONTCARE, 
                    "맑은 고딕", 
                    &m_fonts[tag]);
            }
            break;

        case TAG_FONT::Invetory_Ground:
            {
                AddFontResource(TEXT("Resource/Fonts/SeoulNamsanM.ttf"));
                hr = D3DXCreateFontA(
                    Device()(),
                    14, 7, FW_NORMAL, 1, false,
                    HANGEUL_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    FF_DONTCARE,
                    "08서울남산체 M", &m_fonts[tag]);
            }
            break;
        case TAG_FONT::Invetory_28:
            {
                AddFontResource(TEXT("Resource/Fonts/SeoulNamsanM.ttf"));
                hr = D3DXCreateFontA(
                    Device()(),
                    18, 9, FW_NORMAL, 1, false,
                    HANGEUL_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    FF_DONTCARE,
                    "08서울남산체 M", &m_fonts[tag]);
            }
            break;

        case TAG_FONT::InGameAmmoReload:
            {
                AddFontResource(TEXT("Resource/Fonts/AgencyFBBold.ttf"));
                hr = D3DXCreateFontA(
                    Device()(),
                    26, 10, FW_BOLD, 1, false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    FF_DONTCARE,
                    "Agency FB", &m_fonts[tag]);
            }
            break;
        case TAG_FONT::InGameAmmoTotalNum:
            {
                AddFontResource(TEXT("Resource/Fonts/AgencyFBBold.ttf"));
                hr = D3DXCreateFontA(
                    Device()(),
                    18, 9, FW_NORMAL, 1, false,
                    DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    FF_DONTCARE,
                    "Agency FB", &m_fonts[tag]);
            }
            break;

        case TAG_FONT::InGameSurvival:
        {
            AddFontResource(TEXT("Resource/Fonts/SeoulNamsanM.ttf"));
            hr = D3DXCreateFontA(
                Device()(),
                18, 9, FW_NORMAL, 1, false,
                HANGEUL_CHARSET,
                OUT_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                FF_DONTCARE,
                "08서울남산체 M", &m_fonts[tag]);
        }
        break;

        case TAG_FONT::InGameSurvivalNum:
        {
            AddFontResource(TEXT("Resource/Fonts/SeoulNamsanM.ttf"));
            hr = D3DXCreateFontA(
                Device()(),
                22, 11, FW_NORMAL, 1, false,
                HANGEUL_CHARSET,
                OUT_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                FF_DONTCARE,
                "08서울남산체 M", &m_fonts[tag]);
        }
        break;

        case TAG_FONT::InGameID:
        {
            AddFontResource(TEXT("Resource/Fonts/roboto-condensed.regular.ttf"));
            hr = D3DXCreateFontA(
                Device()(),
                8, 4, FW_NORMAL, 1, false,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                FF_DONTCARE,
                "Roboto Condensed", &m_fonts[tag]);
        }
        break;

        default:
            {
            }
            break;
        }
    }
    else
    {
        hr = 0;
    }

    assert(
        !FAILED(hr) && 
        "Resource::Manager::GetFont(), \
        D3DXCreateFontA() failed.");
    
    return m_fonts[tag];

    //if (m_fonts.find(tag) == m_fonts.end())
    //{
    //	if (val == Font::kIdle)
    //	{
    //		D3DXCreateFont(g_pDevice, 24, 12, FW_NORMAL, 1, false,
    //                          DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
    //                          TEXT("궁서체"), &m_umapFont[val]);
    //	}
    //	else if (val == Font::kQuest)
    //	{
    //		AddFontResource(_T("resources/fonts/umberto.tff"));
    //		D3DXCreateFont(g_pDevice, 24, 12, FW_NORMAL, 1, false,
    //                          DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
    //                          TEXT("umberto"), &m_umapFont[val]);
    //	}
    //       else if (val == Font::kGameOverDescription)
    //       {
    //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    //           D3DXCreateFont(g_pDevice, 36, 18, FW_NORMAL, 1, false,
    //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
    //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
    //       }
    //       else if (val == Font::kGameOverRanking)
    //       {
    //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    //           D3DXCreateFont(g_pDevice, 60, 30, FW_NORMAL, 1, false,
    //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
    //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
    //       }
    //       else if (val == Font::kGameOverRankingNumOther)
    //       {
    //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    //           D3DXCreateFont(g_pDevice, 48, 24, FW_NORMAL, 1, false,
    //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
    //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
    //       }
    //       else if (val == Font::kGameOverNickname)
    //       {
    //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    //           D3DXCreateFont(g_pDevice, 48, 24, FW_NORMAL, 1, false,
    //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
    //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
    //       }
    //       else if (val == Font::kGameOverGoToLobby)
    //       {
    //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    //           D3DXCreateFont(g_pDevice, 36, 18, FW_NORMAL, 1, false,
    //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
    //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
    //       }
    //       else if (val == Font::kInteractionMessageF)
    //       {
    //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    //           D3DXCreateFont(g_pDevice, 16, 8, FW_NORMAL, 1, false,
    //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
    //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
    //       }
    //       else if (val == Font::kInteractionMessageDescription)
    //       {
    //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    //           D3DXCreateFont(g_pDevice, 18, 9, FW_NORMAL, 1, false,
    //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
    //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
    //       }
    //}
    //return m_umapFont[val];
}

SkinnedMesh* Resource::Manager::GetSkinnedMesh(const std::string& fullPath)
{
    const auto search = m_skinnedMeshs.find(fullPath);
    if (search != m_skinnedMeshs.end())
        return search->second;

    return nullptr;
}

SkinnedMesh* Resource::Manager::GetSkinnedMesh(
    const std::string& path, 
    const std::string& filename)
{
    return GetSkinnedMesh(path + filename);
}

EffectMesh* Resource::Manager::GetEffectMesh(const TAG_RES_STATIC tag)
{
    const auto keys = ResourceInfo::GetPathFileName(tag);
    return GetEffectMesh(keys.first, keys.second);
}

EffectMesh* Resource::Manager::GetEffectMesh(
    const std::string& path, 
    const std::string& filename)
{
    const auto search = m_effectMeshs.find(path + filename);
    if (search == m_effectMeshs.end())
    {
        std::string str(filename + " is not found.");
        MessageBoxA(nullptr, str.c_str(), nullptr, MB_OK);
    }

    return search->second;
}

LPDIRECT3DTEXTURE9 Resource::Manager::GetTexture(const std::string& fullPath)
{
    const auto search = m_textures.find(fullPath);
    if (search != m_textures.end())
        return search->second;

    return nullptr;
}

LPDIRECT3DTEXTURE9 Resource::Manager::GetTexture(
    const std::string& path, 
    const std::string& filename)
{
    return GetTexture(path + filename);
}

LPD3DXEFFECT Resource::Manager::GetEffect(const std::string& fullPath)
{
    const auto search = m_effects.find(fullPath);
    if (search != m_effects.end())
        return search->second;

    return nullptr;
}

LPD3DXEFFECT Resource::Manager::GetEffect(
    const std::string& path, 
    const std::string& filename)
{
    return GetEffect(path + filename);
}

LPD3DXMESH Resource::Manager::GetBoundingSphereMesh() const
{
    return m_pBoundingSphereMesh;
}

const std::vector<D3DXVECTOR3>& Resource::Manager::GetBoundingBoxVertices() const
{
    return m_boundingBoxVertices;
}

const std::vector<WORD>& Resource::Manager::GetBoundingBoxIndices() const
{
    return m_boundingBoxIndices;
}

Resource::Manager* Resource::operator()()
{
    return Manager::GetInstance();
}
