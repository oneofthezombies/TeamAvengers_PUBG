#pragma once
#include "Singleton.h"
#include "TagResource.h"
#include "TagClientOnly.h"

#define g_pResource Resource()()

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

        static XContainer* OnLoadTexture(
            const std::string path,
            const std::string xFilename,
            const D3DCOLOR colorKey);

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
        std::map<std::string, LPDIRECT3DTEXTURE9>        m_textures;
        LPD3DXEFFECTPOOL                                 m_pEffectPool;
        std::map<std::string, LPD3DXEFFECT>              m_effects;
        std::map<std::string, EffectMesh*>               m_effectMeshs;
        std::map<TAG_FONT,    LPD3DXFONT>                m_fonts;
        std::map<std::string, std::vector<SkinnedMesh*>> m_skinnedMeshs;
        std::map<std::string, std::size_t>               
            m_availableIndexForSkinnedMesh;

        LPD3DXMESH               m_pBoundingSphereMesh;
        LPD3DXMESH               m_pBoundingBoxMesh;
        std::vector<D3DXVECTOR3> m_boundingBoxVertices;
        std::vector<WORD>        m_boundingBoxIndices;

    public:
                 Manager();
        virtual ~Manager() = default;

        void Init();
        void Destroy();

        void AddResource(XContainer* pXContainer);
        void AddTexture(const string& path, const string& xFilename, const D3DCOLOR colorKey = 0);
        void AddTexture(const TAG_RES_STATIC tag, const D3DCOLOR colorKey = 0);

        LPD3DXFONT GetFont(const TAG_FONT tag);

        SkinnedMesh* GetSkinnedMesh(
            const std::string& fullPath, 
            const std::size_t index);
        SkinnedMesh* GetSkinnedMesh(
            const std::string& path, 
            const std::string& filename, 
            const std::size_t index);

        std::size_t GetNumSkinnedMesh(
            const std::string& path, 
            const std::string& filename);
        std::size_t GetNumSkinnedMesh(const std::string& fullPath);

        bool GetAvailableIndexForSkinnedMesh(
            const std::string& path, 
            const std::string& filename, 
            std::size_t* OutIndex);

        void AddSkinnedMeshCount(
            const std::string& path,
            const std::string& filename);

        EffectMesh* GetEffectMesh(const TAG_RES_STATIC tag);
        EffectMesh* GetEffectMesh(const std::string& pathFilename);
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

              LPD3DXMESH                GetBoundingSphereMesh()  const;
              LPD3DXMESH                GetBoundingBoxMesh() const;
        const std::vector<D3DXVECTOR3>& GetBoundingBoxVertices() const;
        const std::vector<WORD>&        GetBoundingBoxIndices()  const;

        friend Singleton<Manager>;
    };

    Manager* operator()();
};
