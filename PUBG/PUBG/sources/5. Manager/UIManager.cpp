#include "stdafx.h"
#include "UIManager.h"

UIManager::UIManager()
    : SingletonBase<UIManager>()
    , m_pSprite(nullptr)
    , m_UIObjectInstanceID(0)
    , m_bIsDrawBorder(true)
{
}

UIManager::~UIManager()
{
}

void UIManager::Init()
{
    InitSprite();
}

void UIManager::InitSprite()
{
    D3DXCreateSprite(g_pDevice, &m_pSprite);
}

void UIManager::Destroy()
{
    for (auto u : m_usetUIObjects)
    {
        SAFE_RELEASE(u);
        SAFE_DELETE(u);
    }
    SAFE_RELEASE(m_pSprite);
}

void UIManager::Update()
{
    RECT rect;
    GetClientRect(g_hWnd, &rect);
    const int width = rect.right - rect.left;
    const int height = rect.bottom - rect.top;

    D3DXMATRIX mat;
    D3DXMatrixScaling(&mat, static_cast<float>(width) / 1280.0f, static_cast<float>(height) / 720.0f, 0.0f);
    m_pSprite->SetTransform(&mat);

    for (auto& u : m_usetUIObjects)
        SAFE_UPDATE(u);

    UpdateToDeleteUIObjects();
}

void UIManager::Render()
{
    if (!m_pSprite) return;

    const auto dv = g_pDevice;
    dv->SetRenderState(D3DRS_LIGHTING, false);
    dv->SetTexture(0, nullptr);
    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

    for (auto u : m_usetUIObjects)
        SAFE_RENDER(u);

    m_pSprite->End();
    dv->SetRenderState(D3DRS_LIGHTING, true);
}

void UIManager::UpdateToDeleteUIObjects()
{
    const float dt = g_pTimeManager->GetDeltaTime();

    for (auto it = m_umapToDeleteUIObjects.begin(); it != m_umapToDeleteUIObjects.end();)
    {
        auto u = it->first;
        auto& t = it->second;
        
        t -= dt;
        if (t > 0.0f)
        {
            ++it;
        }
        else
        {
            m_usetUIObjects.erase(u);
            SAFE_RELEASE(u);
            SAFE_DELETE(u);
            it = m_umapToDeleteUIObjects.erase(it);
        }
    }
}

void UIManager::RegisterUIObject(UIObject& val)
{
    const auto search = m_usetUIObjects.find(&val);
    if (search != m_usetUIObjects.end()) return;

    val.SetInstanceID(m_UIObjectInstanceID++);
    m_usetUIObjects.emplace(&val);
}

void UIManager::Destroy(UIObject& val, const float t)
{
    const auto search = m_umapToDeleteUIObjects.find(&val);
    if (search != m_umapToDeleteUIObjects.end()) return;

    m_umapToDeleteUIObjects.emplace(&val, t);
}

void UIManager::Destroy(const int instanceID, const float t)
{
    for (auto u : m_usetUIObjects)
    {
        if (u->GetInstanceID() == instanceID)
        {
            Destroy(*u, t);
            break;
        }
    }
}

const LPD3DXSPRITE UIManager::GetSprite() const
{
    return m_pSprite;
}

void UIManager::SetIsDrawBorder(const bool val)
{
    m_bIsDrawBorder = val;
}

bool UIManager::IsDrawBorder() const
{
    return m_bIsDrawBorder;
}
