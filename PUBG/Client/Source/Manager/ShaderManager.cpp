#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
    : Singleton<ShaderManager>()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Init()
{
    m_pGlobalVariableKey = new GlobalVariableKey;
}

void ShaderManager::Destroy()
{
    SAFE_DELETE(m_pGlobalVariableKey);
}

const GlobalVariableKey* const ShaderManager::GetGlobalVariableKey() const
{
    return m_pGlobalVariableKey;
}

GlobalVariableKey::GlobalVariableKey()
    : MemoryAllocator()
    , m_pWorld(new char[strlen("World") + 1])
    , m_pView(new char[strlen("View") + 1])
    , m_pProjection(new char[strlen("Projection") + 1])
    , m_pbEmissiveColor(new char[strlen("bEmissiveColor") + 1])
    , m_pbLight(new char[strlen("bLight") + 1])
    , m_plightDirection(new char[strlen("lightDirection") + 1])
    , m_pDiffuseColor(new char[strlen("DiffuseColor") + 1])
    , m_pSpecularPower(new char[strlen("SpecularPower") + 1])
    , m_pSpecularColor(new char[strlen("SpecularColor") + 1])
    , m_pEmissiveColor(new char[strlen("EmissiveColor") + 1])
{
    std::string world("World");
    m_pWorld = new char[world.size() + 1];
    strcpy_s(m_pWorld, world.size() + 1, world.c_str());

    std::string view("View");
    m_pView = new char[view.size() + 1];
    strcpy_s(m_pView, view.size() + 1, view.c_str());

    std::string projection("Projection");
    m_pProjection = new char[projection.size() + 1];
    strcpy_s(m_pProjection, projection.size() + 1, projection.c_str());

    std::string bEmissiveColor("bEmissiveColor");
    m_pbEmissiveColor = new char[bEmissiveColor.size() + 1];
    strcpy_s(
        m_pbEmissiveColor, 
        bEmissiveColor.size() + 1, 
        bEmissiveColor.c_str());

    std::string bLight("bLight");
    m_pbLight = new char[bLight.size() + 1];
    strcpy_s(m_pbLight, bLight.size() + 1, bLight.c_str());

    std::string lightDirection("lightDirection");
    m_plightDirection = new char[lightDirection.size() + 1];
    strcpy_s(
        m_plightDirection, 
        lightDirection.size() + 1, 
        lightDirection.c_str());

    std::string diffuseColor("DiffuseColor");
    m_pDiffuseColor = new char[diffuseColor.size() + 1];
    strcpy_s(m_pDiffuseColor, diffuseColor.size() + 1, diffuseColor.c_str());

    std::string specularPower("SpecularPower");
    m_pSpecularPower = new char[specularPower.size() + 1];
    strcpy_s(
        m_pSpecularPower, 
        specularPower.size() + 1, 
        specularPower.c_str());

    std::string specularColor("SpecularColor");
    m_pSpecularColor = new char[specularColor.size() + 1];
    strcpy_s(
        m_pSpecularColor, 
        specularColor.size() + 1, 
        specularColor.c_str());

    std::string emissiveColor("EmissiveColor");
    m_pEmissiveColor = new char[emissiveColor.size() + 1];
    strcpy_s(
        m_pEmissiveColor, 
        emissiveColor.size() + 1, 
        emissiveColor.c_str());
}

GlobalVariableKey::~GlobalVariableKey()
{
    SAFE_DELETE_ARRAY(m_pWorld);
    SAFE_DELETE_ARRAY(m_pView);
    SAFE_DELETE_ARRAY(m_pProjection);
    SAFE_DELETE_ARRAY(m_pbEmissiveColor);
    SAFE_DELETE_ARRAY(m_pbLight);
    SAFE_DELETE_ARRAY(m_plightDirection);
    SAFE_DELETE_ARRAY(m_pDiffuseColor);
    SAFE_DELETE_ARRAY(m_pSpecularPower);
    SAFE_DELETE_ARRAY(m_pSpecularColor);
    SAFE_DELETE_ARRAY(m_pEmissiveColor);
}
