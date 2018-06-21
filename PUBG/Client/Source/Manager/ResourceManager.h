#pragma once
#include "ComponentTransform.h"
#include "Singleton.h"

#define g_pResourceManager ResourceManager::GetInstance()

class ResourceManager : public Singleton<ResourceManager>
{
private:
    unordered_map<string, LPDIRECT3DTEXTURE9> m_Textures;
    
    unordered_map<string, LPD3DXEFFECT>       m_Effects;
    LPD3DXEFFECTPOOL                          m_pEffectPool;

    unordered_map<string, EffectMesh*>        m_EffectMeshs;
    unordered_set<SkinnedMesh*>               m_SkinnedMeshs;

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

    SkinnedMesh* GetSkinnedMesh(const string& path, const string& xFilename);
    void RemoveSkinnedMesh(SkinnedMesh* p);

    friend Singleton<ResourceManager>;
};
