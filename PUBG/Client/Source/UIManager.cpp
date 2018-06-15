#include "stdafx.h"
#include "UIManager.h"

UIManager::UIManager()
    : Singleton<UIManager>()
    , m_pSprite(nullptr)
    , m_UIObjectInstanceID(0)
    , m_isDrawBorder(true)
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
    for (auto u : m_UIObjects)
        SAFE_DELETE(u);

    SAFE_RELEASE(m_pSprite);
}

void UIManager::Update()
{
    //RECT rect;
    //GetClientRect(g_hWnd, &rect);
    //const int width = rect.right - rect.left;
    //const int height = rect.bottom - rect.top;

    //D3DXMATRIX transform;
    //D3DXMatrixScaling(&transform, static_cast<float>(width) / 1280.0f, static_cast<float>(height) / 720.0f, 0.0f);
    //m_pSprite->SetTransform(&transform);

    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    D3DXVECTOR3 zero(0.0f, 0.0f, 0.0f);
    for (auto& u : m_UIObjects)
        if (u) u->Update(zero, identity);

    UpdateToDeleteUIObjects();
}

void UIManager::Render()
{
    if (!m_pSprite) return;

    const auto dv = g_pDevice;
    dv->SetRenderState(D3DRS_LIGHTING, false);
    dv->SetTexture(0, nullptr);
    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

    for (auto u : m_UIObjects)
        if (u) u->Render();

    m_pSprite->End();
    dv->SetRenderState(D3DRS_LIGHTING, true);
}

void UIManager::UpdateToDeleteUIObjects()
{
    const float dt = g_pTime->GetDeltaTime();

    const auto begin = m_toDeleteUIObjects.begin();
    const auto end = m_toDeleteUIObjects.end();
    for (auto it = begin; it != end;)
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
            m_UIObjects.erase(u);
            SAFE_DELETE(u);
            it = m_toDeleteUIObjects.erase(it);
        }
    }
}

void UIManager::RegisterUIObject(UIObject* p)
{
    if (!p) return;

    const auto search = m_UIObjects.find(p);
    if (search != m_UIObjects.end()) return;

    p->SetInstanceID(m_UIObjectInstanceID++);
    m_UIObjects.emplace(p);
}

void UIManager::Destroy(UIObject* p, const float t)
{
    if (!p) return;

    const auto search = m_toDeleteUIObjects.find(p);
    if (search != m_toDeleteUIObjects.end()) return;

    m_toDeleteUIObjects.emplace(p, t);
}

void UIManager::Destroy(const int instanceID, const float t)
{
    for (auto u : m_UIObjects)
    {
        if (u->GetInstanceID() == instanceID)
        {
            Destroy(u, t);
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
    m_isDrawBorder = val;
}

bool UIManager::IsDrawBorder() const
{
    return m_isDrawBorder;
}
