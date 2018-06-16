#pragma once
#include "ComponentTransform.h"
#include "Singleton.h"

#define g_pResourceManager ResourceManager::GetInstance()

struct EffectParam
{
    LPD3DXEFFECT       pEffect;
    D3DXHANDLE         hParam;

    EffectParam();
    ~EffectParam();
};

struct EffectMesh
{
    LPD3DXMESH          pMesh;
    vector<EffectParam> effectParams;

    EffectMesh();
    ~EffectMesh();
};

class ResourceManager : public Singleton<ResourceManager>
{
private:
    unordered_map<string, LPDIRECT3DTEXTURE9> m_textures;
    unordered_map<string, LPD3DXEFFECT>       m_effects;
    LPD3DXEFFECTPOOL                          m_pEffectPool;

    unordered_map<string, EffectMesh*>        m_effectMeshs;

    ResourceManager();
    virtual ~ResourceManager();

    EffectMesh* ParseX(const string& path, const string& xFilename);
    
public:
    void Destroy();
    
    LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);
    LPDIRECT3DTEXTURE9 GetTexture(const string& path, 
        const string& textureFilename);
    
    LPD3DXEFFECT GetEffect(const string& fullPath);
    LPD3DXEFFECT GetEffect(const string& path, const string& effectFilename);
    
    EffectMesh* GetEffectMesh(const string& path, const string& xFilename);

    friend Singleton<ResourceManager>;
};

class Renderer : public Component
{
private:
    EffectMesh* m_pEffectMesh;

public:
    Renderer(IObject* pOwner);
    virtual ~Renderer();

    void Render();

    void SetEffectMesh(EffectMesh* p);
};