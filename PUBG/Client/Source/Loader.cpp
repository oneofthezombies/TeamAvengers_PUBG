#include "stdafx.h"
#include "Loader.h"

Renderer::Renderer(IObject* pOwner)
    : Component(pOwner)
    , m_pEffectMesh(nullptr)
{
}

Renderer::~Renderer()
{
}

void Renderer::Render()
{
    for (size_t i = 0u; i < m_pEffectMesh->effectParams.size(); ++i) 
    {
        const auto& ep = m_pEffectMesh->effectParams[i];
        ep.pEffect->ApplyParameterBlock(ep.hParam);

        // TODO :
        D3DXMATRIX s, t, world;
        D3DXMatrixScaling(&s, 1.0f, 1.0f, 1.0f);
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
            m_pEffectMesh->pMesh->DrawSubset(i);
            ep.pEffect->EndPass();
        }
        ep.pEffect->End();
    }
}

void Renderer::SetEffectMesh(EffectMesh* p)
{
    if (!p) return;

    m_pEffectMesh = p;
}

EffectParam::EffectParam()
    : pEffect(nullptr)
    , hParam(nullptr)
{
}

EffectParam::~EffectParam()
{
}

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
    const auto search = m_effects.find(fullPath);
    if (search == m_effects.end())
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

        m_effects[fullPath] = pEffect;
    }
    return m_effects[fullPath];
}

LPD3DXEFFECT ResourceManager::GetEffect(const string& path,
    const string& effectFilename)
{
    return GetEffect(path + effectFilename);
}

/*

TODO : if it is failed, delete pEM

*/
EffectMesh* ResourceManager::ParseX(const string& path, const string& xFilename)
{
    LPD3DXBUFFER pAdjacency = nullptr;
    LPD3DXBUFFER pMaterials = nullptr;
    LPD3DXBUFFER pEffectInstance = nullptr;
    DWORD numMaterials = 0u;

    EffectMesh* pEM = new EffectMesh;

    auto hr = D3DXLoadMeshFromXA((path + xFilename).c_str(), 
        D3DXMESH_MANAGED, g_pDevice,  &pAdjacency, &pMaterials, 
        &pEffectInstance, &numMaterials, &pEM->pMesh);

    if (FAILED(hr))
        SAFE_DELETE(pEM);
    assert(!FAILED(hr) && "ResourceManager::ParseX() failed. D3DXLoadMeshFromXA() failed.");
 
    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE] = { 0 };
    pEM->pMesh->GetDeclaration(decl);
    DWORD declLength = D3DXGetDeclLength(decl);

    bool hasNormals = false;
    for (DWORD i = 0u; i < declLength; ++i)
    {
        if (decl[i].Usage == D3DDECLUSAGE_NORMAL)
        {
            hasNormals = true;
            break;
        }
    }

    //LPD3DXMESH pCloneMesh;
    //hr = pEM->pMesh->CloneMesh(pEM->pMesh->GetOptions(), decl,
    //    g_pDevice, &pCloneMesh);
    //assert(!FAILED(hr) && "ResourceManager::ParseX() failed. CloneMesh() failed.");

    //pEM->pMesh->Release();
    //pEM->pMesh = pCloneMesh;

    if (!hasNormals)
        D3DXComputeNormals(pEM->pMesh, (LPDWORD)pAdjacency->GetBufferPointer());

    pEM->effectParams.resize(numMaterials);

    D3DXMATERIAL* pXMats = (D3DXMATERIAL*)pMaterials->GetBufferPointer();
    D3DXEFFECTINSTANCE* pEI = 
        (D3DXEFFECTINSTANCE*)pEffectInstance->GetBufferPointer();
    
    for (UINT i = 0; i < numMaterials; ++i)
    {
        LPD3DXEFFECT pEffect = GetEffect(path, pEI[i].pEffectFilename);
        pEM->effectParams[i].pEffect = pEffect;

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
        pEM->effectParams[i].hParam = pEffect->EndParameterBlock();
    }
    return pEM;
}

void ResourceManager::Destroy()
{
    SAFE_RELEASE(m_pEffectPool);

    for (auto e : m_effects)
        SAFE_RELEASE(e.second);

    for (auto t : m_textures)
        SAFE_RELEASE(t.second);

    for (auto em : m_effectMeshs)
        SAFE_DELETE(em.second);
}

LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(const string& fullPath)
{
    const auto search = m_textures.find(fullPath);
    if (search == m_textures.end())
    {
        const auto hr = D3DXCreateTextureFromFileExA(g_pDevice, 
            fullPath.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 
            D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
            D3DX_DEFAULT, 0, nullptr, nullptr, &m_textures[fullPath]);

        assert(!FAILED(hr) && "TextureManager::GetTexture() failed.");
    }
    return m_textures[fullPath];
}

LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(const string& path, const string& textureFilename)
{
    return GetTexture(path + textureFilename);
}

EffectMesh* ResourceManager::GetEffectMesh(const string& path, const string& xFilename)
{
    const auto str(path + xFilename);
    const auto search = m_effectMeshs.find(str);
    if (search == m_effectMeshs.end())
    {
        m_effectMeshs[str] = ParseX(path, xFilename);
    }
    return m_effectMeshs[str];
}

EffectMesh::EffectMesh()
    : pMesh(nullptr)
{
}

EffectMesh::~EffectMesh()
{
    SAFE_RELEASE(pMesh);
    effectParams.resize(0);
}
