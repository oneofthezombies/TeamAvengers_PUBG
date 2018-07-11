#pragma once

struct Shader
{
    class Manager : public Singleton<Manager>
    {
    public: 
        static const int SHADOWMAP_DIMENSION = 2048;

    private:
        LPD3DXEFFECT       pCreateShadow;
        LPDIRECT3DTEXTURE9 m_pShadowRenderTarget;
        LPDIRECT3DSURFACE9 m_pShadowDepthStencil;

        std::vector<std::tuple<D3DXMATRIX, LPD3DXMESH, DWORD>> m_shadowSources;

    private:
                 Manager();
        virtual ~Manager();

        void addShadowSource(const D3DXMATRIX& world, LPD3DXFRAME pFrame);
        void addShadowSource(
            const D3DXMATRIX& world,
            LPD3DXMESHCONTAINER pMeshContainer);

    public:
        void Init();
        void Destroy();

        void AddShadowSource(
            const D3DXMATRIX& world,
            LPD3DXMESH pMesh,
            const DWORD attribID);
        void AddShadowSource(
            const D3DXMATRIX& world, 
            SkinnedMesh* pSkinnedMesh);
        void AddShadowSource(
            const D3DXMATRIX& world, 
            EffectMesh* pEffectMesh);

        void ClearShadowSources();
        void CreateShadowMap();

        LPDIRECT3DTEXTURE9 GetShadowRenderTarget() const;

        friend Singleton<Manager>;
    };

    Manager* operator()();

    static const char* World;
    static const char* View;
    static const char* Projection;
    static const char* bEmissiveColor;
    static const char* bLight;
    static const char* lightDirection;
    static const char* DiffuseColor;
    static const char* SpecularPower;
    static const char* SpecularColor;
    static const char* EmissiveColor;

    static void Draw(
        LPD3DXEFFECT pEffect,
        D3DXHANDLE hParam,
        LPD3DXMESH pMesh,
        const DWORD attribID,
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

    static void Draw(
        const std::vector<EffectParam>& effectParams,
        LPD3DXMESH pMesh,
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable);

    static void Draw(
        LPD3DXEFFECT pEffect,
        D3DXHANDLE hParam,
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable,
        const std::function<void()>& drawVertices);

    static void Draw(
        const std::vector<EffectParam>& effectParams,
        const std::function<void(LPD3DXEFFECT)>& setGlobalVariable,
        const std::function<void()>& drawVertices);
};
