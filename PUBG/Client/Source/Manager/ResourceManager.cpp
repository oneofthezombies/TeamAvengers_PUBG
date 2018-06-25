#include "stdafx.h"
#include "ResourceManager.h"
#include "Structure.h"

ResourceManager::ResourceManager()
    : Singleton<ResourceManager>()
    , m_pEffectPool(nullptr)
{
    const auto hr = D3DXCreateEffectPool(&m_pEffectPool);
    assert(!FAILED(hr) && "ResourceManager::Constructor() failed. \
                           D3DXCreateEffectPool() failed.");
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

        const auto hr = D3DXCreateEffectFromFileA(Device()(), 
            fullPath.c_str(), nullptr, nullptr, flags, m_pEffectPool, 
            &pEffect, &pError);

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

EffectMesh* ResourceManager::ParseEffectMeshX(const string& path,
    const string& xFilename)
{
    LPD3DXBUFFER pAdjacency = nullptr;
    LPD3DXBUFFER pMaterials = nullptr;
    LPD3DXBUFFER pEffectInstance = nullptr;
    DWORD numMaterials = 0u;

    EffectMesh* pEM = new EffectMesh;

    auto hr = D3DXLoadMeshFromXA((path + xFilename).c_str(), 
        D3DXMESH_MANAGED, Device()(),  &pAdjacency, 
        &pMaterials, &pEffectInstance, &numMaterials, &pEM->pMesh);

    assert(!FAILED(hr) && 
        "ResourceManager::ParseX() failed. D3DXLoadMeshFromXA() failed.");
 
    D3DXEFFECTINSTANCE* pEI = 
        (D3DXEFFECTINSTANCE*)pEffectInstance->GetBufferPointer();
    
    for (UINT i = 0; i < numMaterials; ++i)
    {
        EffectParam ep;
        LPD3DXEFFECT pEffect = GetEffect(path, pEI[i].pEffectFilename);
        ep.pEffect = pEffect;

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
        ep.hParam = pEffect->EndParameterBlock();
        pEM->EffectParams.emplace_back(ep);
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

    for (auto f : m_fonts)
        SAFE_RELEASE(f.second);

    for (auto em : m_effectMeshs)
        SAFE_DELETE(em.second);

    //RemoveFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
}

LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(const string& fullPath)
{
    const auto search = m_textures.find(fullPath);
    if (search == m_textures.end())
    {
        const auto hr = D3DXCreateTextureFromFileExA(Device()(), 
            fullPath.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 
            D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, 
            D3DX_DEFAULT, 0, nullptr, nullptr, &m_textures[fullPath]);

        assert(!FAILED(hr) && "TextureManager::GetTexture() failed.");
    }
    return m_textures[fullPath];
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
    const auto search = m_effectMeshs.find(str);
    if (search == m_effectMeshs.end())
    {
        m_effectMeshs[str] = ParseEffectMeshX(path, xFilename);
    }
    return m_effectMeshs[str];
}

LPD3DXFONT ResourceManager::GetFont(const TAG_FONT tag)
{
    const auto search = m_fonts.find(tag);
    HRESULT hr = -1;
    if (search == m_fonts.end())
    {
        switch (tag)
        {
        case TAG_FONT::DEFAULT:
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

void ResourceManager::ParseEffectInstances(const string& path,
    const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, 
    EffectMesh* OutEffectMesh)
{
    assert(pEffectInstances && OutEffectMesh && 
        "ResourceManager::ParseEffectInstance(), \
         effect instance or effect mesh is null.");

    auto& pEIs = pEffectInstances;

    for (DWORD ei = 0u; ei < NumMaterials; ++ei)
    {
        const D3DXEFFECTINSTANCE& EI = pEIs[ei];

        if (!EI.pEffectFilename) continue;

        EffectParam EP;
        EP.Name = string(EI.pEffectFilename);
        EP.pEffect = GetEffect(path, EI.pEffectFilename);
        LPD3DXEFFECT& pEffect = EP.pEffect;

        D3DXHANDLE hTech = nullptr;
        pEffect->FindNextValidTechnique(nullptr, &hTech);
        pEffect->SetTechnique(hTech);

        pEffect->BeginParameterBlock();
        for (DWORD di = 0u; di < EI.NumDefaults; ++di)
        {
            auto& d = EI.pDefaults[di];

            D3DXHANDLE h = pEffect->GetParameterByName(nullptr, d.pParamName);
            if (!h) continue;

            D3DXPARAMETER_DESC desc;
            pEffect->GetParameterDesc(h, &desc);

            switch (desc.Type)
            {
            case D3DXPT_TEXTURE:
                {
                    pEffect->SetTexture(
                        d.pParamName, 
                        GetTexture(path, static_cast<char*>(d.pValue)));
                }
                break;
            default:
                {
                    pEffect->SetValue(d.pParamName, d.pValue, d.NumBytes);
                }
                break;
            }
        }
        EP.hParam = pEffect->EndParameterBlock();
        OutEffectMesh->EffectParams.emplace_back(EP);
    }
}
