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

    EffectMesh* ParseEffectMeshX(const string& path, const string& xFilename);
    
public:
    void Destroy();
    
    LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);
    LPDIRECT3DTEXTURE9 GetTexture(const string& path, 
        const string& textureFilename);
    
    LPD3DXEFFECT GetEffect(const string& fullPath);
    LPD3DXEFFECT GetEffect(const string& path, const string& effectFilename);
    
    EffectMesh* GetEffectMesh(const string& path, const string& xFilename);

    LPD3DXFONT GetFont(const TAG_FONT tag);

    void ParseEffectInstances(const string& path, 
        const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, 
        EffectMesh* OutEffectMesh);

    friend Singleton<ResourceManager>;
};

struct Resource
{
    ResourceManager* operator()()
    {
        return ResourceManager::GetInstance();
    }
};