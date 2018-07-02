#include "stdafx.h"
#include "ResourceManager.h"
#include "Structure.h"
#include "Character.h"
#include "ResourceInfo.h"

ResourceManager::ResourceManager()
    : Singleton<ResourceManager>()
    , m_pEffectPool(nullptr)
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init()
{
    HRESULT hr = D3DXCreateEffectPool(&m_pEffectPool);
    assert(!FAILED(hr) && "ResourceManager::Constructor(),\
                           D3DXCreateEffectPool() failed.");

    ResourceContainer* pResourceContainer = new ResourceContainer;

    ResourceAsync::CreateEffect(
        "./Resource/", "Color.fx", 
        pResourceContainer);

    AddResource(pResourceContainer);
}

void ResourceManager::Destroy()
{
    SAFE_RELEASE(m_pEffectPool);

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

LPD3DXFONT ResourceManager::GetFont(const TAG_FONT tag)
{
    const auto search = m_fonts.find(tag);
    HRESULT hr = -1;
    if (search == m_fonts.end())
    {
        switch (tag)
        {
        case TAG_FONT::Default:
            {
                hr = D3DXCreateFont(Device()(), 24, 12, FW_NORMAL, 1, false,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    FF_DONTCARE, TEXT("맑은 고딕"), &m_fonts[tag]);
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
    assert(!FAILED(hr) && "FontManager::GetFont() failed.");
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

SkinnedMesh* ResourceManager::GetSkinnedMesh(const string& path, const string& filename)
{
    const auto search = m_skinnedMeshs.find(path + filename);
    if (search != m_skinnedMeshs.end())
        return search->second;

    return nullptr;
}

EffectMesh* ResourceManager::GetEffectMesh(const TAG_RES_STATIC tag)
{
    const auto keys = ResourceInfo::GetPathFileName(tag);
    const auto search = m_effectMeshs.find(keys.first + keys.second);

    if (search == m_effectMeshs.end())
    {
        std::string str(keys.second + " is not found.");
        MessageBoxA(nullptr, str.c_str(), nullptr, MB_OK);
    }

    return search->second;
}

LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(
    const string& path, const string& filename)
{
    return GetTexture(path + filename);
}

LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(const string& fullPath)
{
    const auto search = m_textures.find(fullPath);
    if (search != m_textures.end())
        return search->second;

    return nullptr;
}

LPD3DXEFFECT ResourceManager::GetEffect(
    const string& path, 
    const string& filename)
{
    return GetEffect(path + filename);
}

LPD3DXEFFECT ResourceManager::GetEffect(const string& fullPath)
{
    const auto search = m_effects.find(fullPath);
    if (search != m_effects.end())
        return search->second;

    return nullptr;
}

void ResourceManager::AddResource(ResourceContainer* pResourceContainer)
{
    assert(pResourceContainer && 
        "ResourceManager::SetResource(), resource container is null.");

    for (auto& kv : pResourceContainer->m_effectMeshs)
    {
        const string& key = kv.first;
        EffectMesh*& pEffectMesh = kv.second;

        const auto search = m_effectMeshs.find(key);
        if (search == m_effectMeshs.end())
        {
            m_effectMeshs[key] = pEffectMesh;
            pEffectMesh = nullptr;
        }
    }

    for (auto& kv : pResourceContainer->m_effects)
    {
        const string& key = kv.first;
        LPD3DXEFFECT& pEffect = kv.second;

        const auto search = m_effects.find(key);
        if (search == m_effects.end())
        {
            m_effects[key] = pEffect;
            pEffect = nullptr;
        }
    }

    for (auto& kv : pResourceContainer->m_textures)
    {
        const string& key = kv.first;
        LPDIRECT3DTEXTURE9& pTexture = kv.second;

        const auto search = m_textures.find(key);
        if (search == m_textures.end())
        {
            m_textures[key] = pTexture;
            pTexture = nullptr;
        }
    }

    if (pResourceContainer->m_pSkinnedMesh.second)
    {
        const string& key = pResourceContainer->m_pSkinnedMesh.first;
        const auto search =
            m_skinnedMeshs.find(key);
        if (search == m_skinnedMeshs.end())
        {
            m_skinnedMeshs[key] = pResourceContainer->m_pSkinnedMesh.second;
            pResourceContainer->m_pSkinnedMesh.second = nullptr;

            m_skinnedMeshs[key]->Setup();
        }
    }

    SAFE_DELETE(pResourceContainer);
}

ResourceContainer::ResourceContainer()
    : m_filename("")
{
}

ResourceContainer::~ResourceContainer()
{
    for (auto& kv : m_effectMeshs)
    {
        auto& em = kv.second;
        SAFE_DELETE(em);
    }

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

    SAFE_DELETE(m_pSkinnedMesh.second);
}

ResourceContainer* ResourceAsync::OnLoadEffectMeshAsync(
    const string path, const string xFilename)
{
    LPD3DXBUFFER pEffectInstancesBuffer = nullptr;
    LPD3DXMESH   pMesh = nullptr;
    DWORD        numMaterials = 0u;

    HRESULT hr = D3DXLoadMeshFromXA((path + xFilename).c_str(), 
        D3DXMESH_MANAGED, Device()(), nullptr, nullptr, 
        &pEffectInstancesBuffer, &numMaterials, &pMesh);

    if (FAILED(hr))
    {
        return nullptr;
    }

    D3DXEFFECTINSTANCE* pEffectInstances =
        static_cast<D3DXEFFECTINSTANCE*>(
            pEffectInstancesBuffer->GetBufferPointer());

    ResourceContainer* pResourceContainer = new ResourceContainer;

    hr = CreateEffectMesh(path, xFilename, pMesh, pEffectInstances, 
        numMaterials, pResourceContainer);

    if (FAILED(hr))
    {
        SAFE_DELETE(pResourceContainer);
        return nullptr;
    }

    pResourceContainer->m_filename = xFilename;
    return pResourceContainer;
}

ResourceContainer* ResourceAsync::OnLoadSkinnedMeshAsync(
    const string path, const string xFilename)
{
    ResourceContainer* pResourceContainer = new ResourceContainer;
    if (!pResourceContainer)
    {
        return nullptr;
    }

    pResourceContainer->m_pSkinnedMesh = 
        make_pair(path + xFilename, new SkinnedMesh);
    SkinnedMesh* pSkinnedMesh = pResourceContainer->m_pSkinnedMesh.second;
    if (!pSkinnedMesh)
    {
        return nullptr;
    }

    AllocateHierarchyAsync allocAsync(path, xFilename, pResourceContainer);

    HRESULT hr = D3DXLoadMeshHierarchyFromXA((path + xFilename).c_str(),
        D3DXMESH_MANAGED, Device()(), &allocAsync, nullptr, 
        &pSkinnedMesh->m_pRootFrame,
        &pSkinnedMesh->m_pAnimController);

    if (FAILED(hr))
    {
        return nullptr;
    }

    assert(pResourceContainer->m_pSkinnedMesh.second && 
        pResourceContainer->m_pSkinnedMesh.second->m_pRootFrame && 
        pResourceContainer->m_pSkinnedMesh.second->m_pAnimController && 
        "@@");

    pResourceContainer->m_filename = xFilename;
    return pResourceContainer;
}

HRESULT ResourceAsync::CreateEffectMesh(const string& path, const string& name,
    LPD3DXMESH pMesh, const D3DXEFFECTINSTANCE* pEffectInstances, 
    const DWORD numEffectInstances, ResourceContainer* OutResourceContainer)
{
    if (pMesh == nullptr || 
        pEffectInstances == nullptr || 
        OutResourceContainer == nullptr)
    {
        return E_FAIL;
    }

    const string key = path + name;
    const auto searchResult = OutResourceContainer->m_effectMeshs.find(key);
    if (searchResult != OutResourceContainer->m_effectMeshs.end())
    {
        return S_OK;
    }

    EffectMesh* pEffectMesh = new EffectMesh;
    OutResourceContainer->m_effectMeshs[path + name] = pEffectMesh;

    pEffectMesh->m_pMesh = pMesh;
    /**/
    //TODO
    void* pData = nullptr;
    pEffectMesh->m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pData);
    D3DXComputeBoundingSphere(
        (D3DXVECTOR3*)pData, 
        pEffectMesh->m_pMesh->GetNumVertices(), 
        pEffectMesh->m_pMesh->GetNumBytesPerVertex(), 
        &pEffectMesh->m_center, 
        &pEffectMesh->m_radius);
    pEffectMesh->m_pMesh->UnlockVertexBuffer();
    //cout << name << " " << pEffectMesh->m_center << ", " << pEffectMesh->m_radius << endl;
    /**/

    for (DWORD i = 0u; i < numEffectInstances; ++i)
    {
        const D3DXEFFECTINSTANCE& EI = pEffectInstances[i];

        if (EI.pEffectFilename == false) continue;

        EffectParam effectParam;
        effectParam.name = string(EI.pEffectFilename);
        
        HRESULT hr = CreateEffect(
            path, EI.pEffectFilename, OutResourceContainer);

        if (FAILED(hr))
        {
            return E_FAIL;
        }

        effectParam.pEffect = 
            OutResourceContainer->m_effects[path + string(EI.pEffectFilename)];

        D3DXHANDLE hTech = nullptr;
        LPD3DXEFFECT& pEffect = effectParam.pEffect;
        
        pEffect->FindNextValidTechnique(nullptr, &hTech);
        pEffect->SetTechnique(hTech);

        pEffect->BeginParameterBlock();
        for (DWORD di = 0u; di < EI.NumDefaults; ++di)
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
                    const string textureFilename(
                        static_cast<char*>(effectDefault.pValue));
                    
                    hr = CreateTexture(path, textureFilename, 
                        OutResourceContainer);

                    if (FAILED(hr))
                    {
                        return E_FAIL;
                    }

                    auto& textures = OutResourceContainer->m_textures;

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

HRESULT ResourceAsync::CreateEffect(const string& path, const string& filename,
    ResourceContainer* OutResourceContainer)
{
    if (OutResourceContainer == nullptr)
    {
        return E_FAIL;
    }

    const string key = path + filename;
    const auto searchResult = OutResourceContainer->m_effects.find(key);
    if (searchResult != OutResourceContainer->m_effects.end())
    {
        return S_OK;
    }

    LPD3DXEFFECT pEffect = nullptr;
    LPD3DXBUFFER pError = nullptr;
    DWORD flags = D3DXSHADER_DEBUG | D3DXFX_NOT_CLONEABLE;
    HRESULT hr = D3DXCreateEffectFromFileA(Device()(), key.c_str(), nullptr, 
        nullptr, flags, nullptr, &pEffect, &pError);

    if (FAILED(hr))
    {
        if (pError)
        {
            string errorMessage(
                static_cast<char*>(pError->GetBufferPointer()), 
                pError->GetBufferSize());

            MessageBoxA(nullptr, errorMessage.c_str(), nullptr, MB_OK);
        }

        return E_FAIL;
    }

    OutResourceContainer->m_effects[key] = pEffect;
    return S_OK;
}

HRESULT ResourceAsync::CreateTexture(
    const string& path, const string& filename, 
    ResourceContainer* OutResourceContainer)
{
    if (OutResourceContainer == nullptr)
    {
        return E_FAIL;
    }

    const string key(path + filename);
    const auto searchResult = OutResourceContainer->m_textures.find(key);
    if (searchResult != OutResourceContainer->m_textures.end())
    {
        return S_OK;
    }

    LPDIRECT3DTEXTURE9 pTexture = nullptr;
    const HRESULT hr = D3DXCreateTextureFromFileExA(Device()(), key.c_str(), 
        D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, 
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
        nullptr, nullptr, &pTexture);

    if (FAILED(hr))
    {
        return E_FAIL;
    }

    OutResourceContainer->m_textures[key] = pTexture;
    return S_OK;
}
