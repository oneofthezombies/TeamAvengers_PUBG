#pragma once

struct Shader
{
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
