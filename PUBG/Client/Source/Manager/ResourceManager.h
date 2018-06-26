#pragma once
#include "ComponentTransform.h"
#include "Singleton.h"

struct ResourceContainer
{
    unordered_map<string, LPDIRECT3DTEXTURE9> m_textures;
    unordered_map<string, LPD3DXEFFECT>       m_effects;
    unordered_map<string, EffectMesh*>        m_effectMeshs;
    SkinnedMesh*                              m_pSkinnedMesh;

    ResourceContainer();
    ~ResourceContainer();
};

class ResourceManager : public Singleton<ResourceManager>
{
private:
    unordered_map<string, LPDIRECT3DTEXTURE9> m_textures;
    
    unordered_map<string, LPD3DXEFFECT> m_effects;
    LPD3DXEFFECTPOOL                    m_pEffectPool;
    unordered_map<string, EffectMesh*>  m_effectMeshs;

    unordered_map<TAG_FONT, LPD3DXFONT> m_fonts;

    array<SkinnedMesh*, 4> m_characters;

private:
             ResourceManager();
    virtual ~ResourceManager();

public:
    void Destroy();
    void LoadAll();

    EffectMesh* FindEffectMesh(const string& path, const string& name);
    EffectMesh* AddEffectMesh(const string& path, const string& name,
        LPD3DXMESH pMesh, const D3DXEFFECTINSTANCE* pEffectInstances,
        DWORD numMaterials);
    void AddResource(ResourceContainer* pResourceContainer);
    void AddCharacters(
        std::vector<ResourceContainer*>* OutCharacters);

public:
    LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);
    LPDIRECT3DTEXTURE9 GetTexture(const string& path,
        const string& textureFilename);

    LPD3DXEFFECT GetEffect(const string& fullPath);
    LPD3DXEFFECT GetEffect(const string& path, const string& effectFilename);

    EffectMesh* GetEffectMesh(const string& path, const string& xFilename);

    LPD3DXFONT GetFont(const TAG_FONT tag);

public:

    friend Singleton<ResourceManager>;
};

struct Resource
{
    ResourceManager* operator()()
    {
        return ResourceManager::GetInstance();
    }
};

struct ResourceAsync
{
    static ResourceContainer* OnLoadEffectMeshAsync(
        const string path, const string xFilename);
    
    static ResourceContainer* OnLoadSkinnedMeshAsync(
        const string path, const string xFilename);

    static HRESULT CreateEffectMesh(const string& path, const string& name,
        LPD3DXMESH pMesh, const D3DXEFFECTINSTANCE* pEffectInstances,
        const DWORD numEffectInstances, 
        ResourceContainer* OutResourceContainer);

    static HRESULT CreateEffect(const string& path, const string& filename,
        ResourceContainer* OutResourceContainer);

    static HRESULT CreateTexture(const string& path, const string& filename,
        ResourceContainer* OutResourceContainer);
};


