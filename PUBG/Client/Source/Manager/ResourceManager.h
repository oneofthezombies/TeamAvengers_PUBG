#pragma once
#include "ComponentTransform.h"
#include "Singleton.h"

class ResourceManager : public Singleton<ResourceManager>
{
private:
    unordered_map<string, LPDIRECT3DTEXTURE9> m_textures;
    
    unordered_map<string, LPD3DXEFFECT>       m_effects;
    LPD3DXEFFECTPOOL                          m_pEffectPool;

    unordered_map<string, EffectMesh*>        m_effectMeshs;

    unordered_map<TAG_FONT, LPD3DXFONT>       m_fonts;

             ResourceManager();
    virtual ~ResourceManager();

public:
    void Destroy();

    EffectMesh* FindEffectMesh(const string& path, const string& name);
    EffectMesh* AddEffectMesh(const string& path, const string& name,
        LPD3DXMESH pMesh, const D3DXEFFECTINSTANCE* pEffectInstances,
        DWORD numMaterials);

public:
    LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);
    LPDIRECT3DTEXTURE9 GetTexture(const string& path,
        const string& textureFilename);

    LPD3DXEFFECT GetEffect(const string& fullPath);
    LPD3DXEFFECT GetEffect(const string& path, const string& effectFilename);

    EffectMesh* GetEffectMesh(const string& path, const string& xFilename);

    LPD3DXFONT GetFont(const TAG_FONT tag);

    friend Singleton<ResourceManager>;
};

struct Resource
{
    ResourceManager* operator()()
    {
        return ResourceManager::GetInstance();
    }
};