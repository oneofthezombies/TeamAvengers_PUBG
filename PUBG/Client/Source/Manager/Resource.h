#pragma once
#include "Singleton.h"
#include "TagResource.h"
#include "TagClientOnly.h"

#define g_pResource Resource()()

//struct ResourceContainer
//{
//    std::string m_filename;
//
//    std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_textures;
//    std::unordered_map<std::string, LPD3DXEFFECT>       m_effects;
//    std::unordered_map<std::string, EffectMesh*>        m_effectMeshs;
//
//    std::pair<std::string, SkinnedMesh*> m_pSkinnedMesh;
//
//    ResourceContainer();
//    ~ResourceContainer();
//};

//class ResourceManager : public Singleton<ResourceManager>
//{
//private:
//    unordered_map<string, LPDIRECT3DTEXTURE9> m_textures;
//    
//    unordered_map<string, LPD3DXEFFECT> m_effects;
//    LPD3DXEFFECTPOOL                    m_pEffectPool;
//    unordered_map<string, EffectMesh*>  m_effectMeshs;
//    unordered_map<string, SkinnedMesh*> m_skinnedMeshs;
//
//    unordered_map<TAG_FONT, LPD3DXFONT> m_fonts;
//
//private:
//             ResourceManager();
//    virtual ~ResourceManager();
//
//public:
//    void Init();
//    void Destroy();
//
//    void AddResource(ResourceContainer* pResourceContainer);
//
//    LPD3DXFONT GetFont(const TAG_FONT tag);
//
//    SkinnedMesh* GetSkinnedMesh(const string& path, const string& filename);
//    EffectMesh*  GetEffectMesh(const TAG_RES_STATIC tag);
//    LPDIRECT3DTEXTURE9 GetTexture(const string& path, const string& filename);
//    LPDIRECT3DTEXTURE9 GetTexture(const string& fullPath);
//    LPD3DXEFFECT GetEffect(const string& path, const string& filename);
//    LPD3DXEFFECT GetEffect(const string& fullPath);
//
//    friend Singleton<ResourceManager>;
//};

struct Resource
{
    enum class Policy
    {
        SYNC,
        ASYNC,
    };

    struct XContainer
    {
        std::string                                         m_filename;
        std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_textures;
        std::unordered_map<std::string, LPD3DXEFFECT>       m_effects;
        std::unordered_map<std::string, EffectMesh*>        m_effectMeshs;
        std::pair         <std::string, SkinnedMesh*>       m_pSkinnedMesh;

         XContainer();
        ~XContainer();
    };

    struct Async
    {
        class AllocateHierarchy : public ID3DXAllocateHierarchy
        {
        private:
            std::string m_path;
            std::string m_xFilename;
            XContainer* pXContainer;

        public:
             AllocateHierarchy();
             AllocateHierarchy(
                 const std::string path,
                 const std::string xFilename,
                       XContainer* OutXContainer);
            ~AllocateHierarchy() = default;

            STDMETHOD(CreateFrame)(
                THIS_ LPCSTR       Name,
                      LPD3DXFRAME* ppNewFrame) override;

            STDMETHOD(CreateMeshContainer)(
                THIS_ LPCSTR               Name,
                CONST D3DXMESHDATA*        pMeshData,
                CONST D3DXMATERIAL*        pMaterials,
                CONST D3DXEFFECTINSTANCE*  pEffectInstances,
                      DWORD                NumMaterials,
                CONST DWORD*               pAdjacency,
                      LPD3DXSKININFO       pSkinInfo,
                      LPD3DXMESHCONTAINER* ppNewMeshContainer) override;

            STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameBase) override;

            STDMETHOD(DestroyMeshContainer)(
                THIS_ LPD3DXMESHCONTAINER pMeshContainerBase) override;
        };

        static XContainer* OnLoadEffectMesh(
            const std::string path, 
            const std::string xFilename);

        static XContainer* OnLoadSkinnedMesh(
            const std::string path, 
            const std::string xFilename);

        static HRESULT CreateEffectMesh(
            const std::string         path, 
            const std::string         name,
                  LPD3DXMESH          pMesh, 
            const D3DXEFFECTINSTANCE* pEffectInstances,
            const DWORD               numEffectInstances,
                  XContainer*         OutXContainer);

        static HRESULT CreateEffect(
            const std::string path, 
            const std::string filename,
                  XContainer* OutXContainer);

        static HRESULT CreateTexture(
            const std::string path, 
            const std::string filename,
                  XContainer* OutXContainer);
    };

    class Manager : public Singleton<Manager>
    {
    private:
        std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_textures;
        LPD3DXEFFECTPOOL                                    m_pEffectPool;
        std::unordered_map<std::string, LPD3DXEFFECT>       m_effects;
        std::unordered_map<std::string, EffectMesh*>        m_effectMeshs;
        std::unordered_map<std::string, SkinnedMesh*>       m_skinnedMeshs;
        std::unordered_map<TAG_FONT,    LPD3DXFONT>         m_fonts;

    public:
                 Manager();
        virtual ~Manager() = default;

        void Init();
        void Destroy();

        void AddResource(XContainer* pXContainer);

        LPD3DXFONT GetFont(const TAG_FONT tag);

        SkinnedMesh* GetSkinnedMesh(const std::string& fullPath);
        SkinnedMesh* GetSkinnedMesh(
            const std::string& path, 
            const std::string& filename);

        EffectMesh* GetEffectMesh(const TAG_RES_STATIC tag);
        EffectMesh* GetEffectMesh(
            const std::string& path, 
            const std::string& filename);

        LPDIRECT3DTEXTURE9 GetTexture(const std::string& fullPath);
        LPDIRECT3DTEXTURE9 GetTexture(
            const std::string& path, 
            const std::string& filename);

        LPD3DXEFFECT GetEffect(const std::string& fullPath);
        LPD3DXEFFECT GetEffect(
            const std::string& path, 
            const std::string& filename);

        friend Singleton<Manager>;
    };

    Manager* operator()();
};

//struct ResourceAsync
//{
//    static ResourceContainer* OnLoadEffectMeshAsync(
//        const string path, const string xFilename);
//    
//    static ResourceContainer* OnLoadSkinnedMeshAsync(
//        const string path, const string xFilename);
//
//    static HRESULT CreateEffectMesh(const string path, const string name,
//        LPD3DXMESH pMesh, const D3DXEFFECTINSTANCE* pEffectInstances,
//        const DWORD numEffectInstances, 
//        ResourceContainer* OutResourceContainer);
//
//    static HRESULT CreateEffect(const string path, const string filename,
//        ResourceContainer* OutResourceContainer);
//
//    static HRESULT CreateTexture(const string path, const string filename,
//        ResourceContainer* OutResourceContainer);
//};


