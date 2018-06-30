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

    unordered_map<TAG_FONT, LPD3DXFONT> m_fonts;

    SkinnedMesh*                        m_pCharacter;
    array<LPD3DXANIMATIONCONTROLLER, 4> m_characterAnimControllers;

    unordered_map<string, SkinnedMesh*> m_skinnedMeshs;
    unordered_map<SkinnedMesh*, LPD3DXANIMATIONCONTROLLER> m_animationController;

private:
             ResourceManager();
    virtual ~ResourceManager();

public:
    void Init();
    void Destroy();

    void AddResource(ResourceContainer* pResourceContainer);
    
    void AddCharacter(ResourceContainer* pResourceContainer);

    LPD3DXFONT GetFont(const TAG_FONT tag);

    SkinnedMesh* GetSkinnedMesh(const string& path, const string& filename);
    SkinnedMesh* GetCharacterSkinnedMesh();
    EffectMesh*  GetEffectMesh(const TAG_RES_STATIC tag);
    LPDIRECT3DTEXTURE9 GetTexture(const string& path, const string& filename);
    LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);
    LPD3DXEFFECT GetEffect(const string& path, const string& filename);
    LPD3DXEFFECT GetEffect(const string& fullPath);

    friend Singleton<ResourceManager>;

    ///* do NOT use! this will be deleted soon.*/
    //EffectMesh* FindEffectMesh(const string& path, const string& name);

    ///* do NOT use! this will be deleted soon.*/
    //EffectMesh* AddEffectMesh(const string& path, const string& name,
    //    LPD3DXMESH pMesh, const D3DXEFFECTINSTANCE* pEffectInstances,
    //    DWORD numMaterials);

    ///* do NOT use! this will be deleted soon.*/
    //LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);

    ///* do NOT use! this will be deleted soon.*/
    //LPDIRECT3DTEXTURE9 GetTexture(const string& path,
    //    const string& textureFilename);

    ///* do NOT use! this will be deleted soon.*/
    //LPD3DXEFFECT GetEffect(const string& fullPath);

    ///* do NOT use! this will be deleted soon.*/
    //LPD3DXEFFECT GetEffect(const string& path, const string& effectFilename);

    ///* do NOT use! this will be deleted soon.*/
    //EffectMesh* GetEffectMesh(const string& path, const string& xFilename);
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


