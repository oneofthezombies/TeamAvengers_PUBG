#pragma once
#include "Singleton.h"

struct GlobalVariableKey : public MemoryAllocator
{
    char* m_pWorld;
    char* m_pView;
    char* m_pProjection;
    char* m_pbEmissiveColor;
    char* m_pbLight;
    char* m_plightDirection;
    char* m_pDiffuseColor;
    char* m_pSpecularPower;
    char* m_pSpecularColor;
    char* m_pEmissiveColor;

    GlobalVariableKey();
    virtual ~GlobalVariableKey();
};

class ShaderManager : public Singleton<ShaderManager>
{
private:
    GlobalVariableKey* m_pGlobalVariableKey;

             ShaderManager();
    virtual ~ShaderManager();

public:
    void Init();
    void Destroy();

    const GlobalVariableKey* const GetGlobalVariableKey() const;

    friend Singleton<ShaderManager>;
};

struct Shader
{
    ShaderManager* operator()()
    {
        return ShaderManager::GetInstance();
    }
};