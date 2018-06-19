#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
    : Singleton<ResourceManager>()
    , m_pEffectPool(nullptr)
{
    const auto hr = D3DXCreateEffectPool(&m_pEffectPool);
    assert(!FAILED(hr) && "ResourceManager::Constructor() failed. D3DXCreateEffectPool() failed.");
}

ResourceManager::~ResourceManager()
{
}

LPD3DXEFFECT ResourceManager::GetEffect(const string& fullPath)
{
    const auto search = m_Effects.find(fullPath);
    if (search == m_Effects.end())
    {
        LPD3DXEFFECT pEffect = nullptr;
        LPD3DXBUFFER pError = nullptr;
        DWORD flags = D3DXSHADER_DEBUG | D3DXFX_NOT_CLONEABLE;

        const auto hr = D3DXCreateEffectFromFileA(g_pDevice, fullPath.c_str(), 
            nullptr, nullptr, flags, m_pEffectPool, &pEffect, &pError);

        if (!pEffect && pError)
        {
            const auto size = pError->GetBufferSize();
            const auto errorMsg = pError->GetBufferPointer();

            string str((char*)errorMsg, size);
            str = string("ResourceManager::LoadEffect() failed. ") + str;
            assert(false && str.c_str());
        }

        m_Effects[fullPath] = pEffect;
    }
    return m_Effects[fullPath];
}

LPD3DXEFFECT ResourceManager::GetEffect(const string& path,
    const string& effectFilename)
{
    return GetEffect(path + effectFilename);
}

EffectMesh* ResourceManager::ParseEffectMeshX(const string& path, 
    const string& xFilename)
{
    LPD3DXBUFFER pAdjacency = nullptr;
    LPD3DXBUFFER pMaterials = nullptr;
    LPD3DXBUFFER pEffectInstance = nullptr;
    DWORD numMaterials = 0u;

    EffectMesh* pEM = new EffectMesh;

    auto hr = D3DXLoadMeshFromXA((path + xFilename).c_str(), 
        D3DXMESH_MANAGED, g_pDevice,  &pAdjacency, &pMaterials, 
        &pEffectInstance, &numMaterials, &pEM->pMesh);

    assert(!FAILED(hr) && "ResourceManager::ParseX() failed. D3DXLoadMeshFromXA() failed.");
 
    //D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE] = { 0 };
    //pEM->pMesh->GetDeclaration(decl);
    //DWORD declLength = D3DXGetDeclLength(decl);

    //bool hasNormals = false;
    //for (DWORD i = 0u; i < declLength; ++i)
    //{
    //    if (decl[i].Usage == D3DDECLUSAGE_NORMAL)
    //    {
    //        hasNormals = true;
    //        break;
    //    }
    //}

    //LPD3DXMESH pCloneMesh;
    //hr = pEM->pMesh->CloneMesh(pEM->pMesh->GetOptions(), decl,
    //    g_pDevice, &pCloneMesh);
    //assert(!FAILED(hr) && "ResourceManager::ParseX() failed. CloneMesh() failed.");

    //pEM->pMesh->Release();
    //pEM->pMesh = pCloneMesh;

    //if (!hasNormals)
    //    D3DXComputeNormals(pEM->pMesh, (LPDWORD)pAdjacency->GetBufferPointer());

    pEM->EffectParams.resize(numMaterials);

    //D3DXMATERIAL* pXMats = (D3DXMATERIAL*)pMaterials->GetBufferPointer();

    D3DXEFFECTINSTANCE* pEI = 
        (D3DXEFFECTINSTANCE*)pEffectInstance->GetBufferPointer();
    
    for (UINT i = 0; i < numMaterials; ++i)
    {
        LPD3DXEFFECT pEffect = GetEffect(path, pEI[i].pEffectFilename);
        pEM->EffectParams[i].pEffect = pEffect;

        D3DXHANDLE hTech;
        pEffect->FindNextValidTechnique(nullptr, &hTech);
        pEffect->SetTechnique(hTech);

        pEffect->BeginParameterBlock();
        for (UINT p = 0u; p < pEI[i].NumDefaults; ++p)
        {
            const auto pDefault = pEI[i].pDefaults[p];
            D3DXHANDLE hHandle = pEffect->GetParameterByName(nullptr, 
                pDefault.pParamName);

            D3DXPARAMETER_DESC desc;
            if (hHandle)
            {
                pEffect->GetParameterDesc(hHandle, &desc);
                
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
                        GetTexture(path, string((char*)pDefault.pValue)));
                }
            }
        }
        pEM->EffectParams[i].hParam = pEffect->EndParameterBlock();
    }
    return pEM;
}

void ResourceManager::Destroy()
{
    SAFE_RELEASE(m_pEffectPool);

    for (auto e : m_Effects)
        SAFE_RELEASE(e.second);

    for (auto t : m_Textures)
        SAFE_RELEASE(t.second);

    for (auto em : m_EffectMeshs)
        SAFE_DELETE(em.second);

    for (auto af : m_SkinnedMeshs)
        SAFE_DELETE(af.second);
}

LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(const string& fullPath)
{
    const auto search = m_Textures.find(fullPath);
    if (search == m_Textures.end())
    {
        const auto hr = D3DXCreateTextureFromFileExA(g_pDevice, 
            fullPath.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 
            D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
            D3DX_DEFAULT, 0, nullptr, nullptr, &m_Textures[fullPath]);

        assert(!FAILED(hr) && "TextureManager::GetTexture() failed.");
    }
    return m_Textures[fullPath];
}

LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(const string& path, 
    const string& textureFilename)
{
    return GetTexture(path + textureFilename);
}

EffectMesh* ResourceManager::GetEffectMesh(const string& path, 
    const string& xFilename)
{
    const auto str(path + xFilename);
    const auto search = m_EffectMeshs.find(str);
    if (search == m_EffectMeshs.end())
    {
        m_EffectMeshs[str] = ParseEffectMeshX(path, xFilename);
    }
    return m_EffectMeshs[str];
}

SkinnedMesh* ResourceManager::GetSkinnedMesh(const string& path,
    const string& xFilename)
{
    const auto str(path + xFilename);
    const auto search = m_SkinnedMeshs.find(str);
    if (search == m_SkinnedMeshs.end())
    {
        SkinnedMesh* pSkinnedMesh = new SkinnedMesh;
        AllocateHierarchy ah(path, xFilename);

        const auto hr = D3DXLoadMeshHierarchyFromXA(str.c_str(), 
            D3DXMESH_MANAGED, g_pDevice, &ah, nullptr,
            &pSkinnedMesh->pRootFrame, &pSkinnedMesh->pAnimController);

        assert(!FAILED(hr) && "ResourceManager::GetSkinnedMesh() failed.");

        pSkinnedMesh->SetupBoneMatrixPointers(pSkinnedMesh->pRootFrame);

        m_SkinnedMeshs[str] = pSkinnedMesh;
    }
    return m_SkinnedMeshs[str];
}
