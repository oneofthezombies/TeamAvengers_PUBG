#pragma once
#include "ComponentTransform.h"
#include "Singleton.h"
#include "TagResource.h"

#define g_pResource Resource()()

struct ResourceContainer
{
    std::string m_filename;

    std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_textures;
    std::unordered_map<std::string, LPD3DXEFFECT>       m_effects;
    std::unordered_map<std::string, EffectMesh*>        m_effectMeshs;

    std::pair<std::string, SkinnedMesh*> m_pSkinnedMesh;

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
    unordered_map<string, SkinnedMesh*> m_skinnedMeshs;

    unordered_map<TAG_FONT, LPD3DXFONT> m_fonts;

private:
             ResourceManager();
    virtual ~ResourceManager();

public:
    void Init();
    void Destroy();

    void AddResource(ResourceContainer* pResourceContainer);

    LPD3DXFONT GetFont(const TAG_FONT tag);

    SkinnedMesh* GetSkinnedMesh(const string& path, const string& filename);
    EffectMesh*  GetEffectMesh(const TAG_RES_STATIC tag);
    LPDIRECT3DTEXTURE9 GetTexture(const string& path, const string& filename);
    LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);
    LPD3DXEFFECT GetEffect(const string& path, const string& filename);
    LPD3DXEFFECT GetEffect(const string& fullPath);

    friend Singleton<ResourceManager>;
};

struct Resource
{
    enum class Policy
    {
        SYNC,
        ASYNC,
    };

    struct Container
    {

    };

    struct Async
    {

    };

    struct Info
    {

    };

    class Manager
    {

    };

    ResourceManager* operator()()
    {
        return ResourceManager::GetInstance();
    }

    static void Load(const Policy policy) {}
};

struct ResourceAsync
{
    static ResourceContainer* OnLoadEffectMeshAsync(
        const string path, const string xFilename);
    
    static ResourceContainer* OnLoadSkinnedMeshAsync(
        const string path, const string xFilename);

    static HRESULT CreateEffectMesh(const string path, const string name,
        LPD3DXMESH pMesh, const D3DXEFFECTINSTANCE* pEffectInstances,
        const DWORD numEffectInstances, 
        ResourceContainer* OutResourceContainer);

    static HRESULT CreateEffect(const string path, const string filename,
        ResourceContainer* OutResourceContainer);

    static HRESULT CreateTexture(const string path, const string filename,
        ResourceContainer* OutResourceContainer);
};


