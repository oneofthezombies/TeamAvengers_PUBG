#include "stdafx.h"
#include "UIObject.h"
#include "UIManager.h"

UIObject::UIObject(UIObject* pParent)
    : MemoryAllocator()
    , m_instanceID(-1)
    , m_color(D3DCOLOR_XRGB(255, 255, 255))
    , m_position()
    , m_center()
    , m_viewportPosition()
    , m_size()
    , m_pAttachedObject(nullptr)
{
    D3DXMatrixIdentity(&m_transform);

    if (pParent)
    {
        pParent->AddChild(this);
    }
    else
    {
        UI()()->RegisterUIObject(this);
    }
}

UIObject::~UIObject()
{
    for (auto c : m_children)
        SAFE_DELETE(c);
}

void UIObject::DrawBorder()
{
    if (!UI()()->IsDrawBorder()) return;

    D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255);
    vector<VERTEX_RHWC> vertices = 
    {
        VERTEX_RHWC(static_cast<float>(m_rect.left),  static_cast<float>(m_rect.top),    0.0f, 1.0f, c),
        VERTEX_RHWC(static_cast<float>(m_rect.right), static_cast<float>(m_rect.top),    0.0f, 1.0f, c),
        VERTEX_RHWC(static_cast<float>(m_rect.right), static_cast<float>(m_rect.bottom), 0.0f, 1.0f, c),
        VERTEX_RHWC(static_cast<float>(m_rect.left),  static_cast<float>(m_rect.bottom), 0.0f, 1.0f, c),
        VERTEX_RHWC(static_cast<float>(m_rect.left),  static_cast<float>(m_rect.top),    0.0f, 1.0f, c),
    };
    
    const auto d = Device()();
    d->SetFVF(VERTEX_RHWC::FVF);
    d->DrawPrimitiveUP(D3DPT_LINESTRIP, vertices.size() - 1, vertices.data(), sizeof VERTEX_RHWC);
}

void UIObject::SetViewportPosition(const D3DXVECTOR3& parentViewportPos, const D3DXVECTOR3& pos)
{
    m_viewportPosition = parentViewportPos + pos;
}

void UIObject::SetRect(const D3DXVECTOR3& VPos, const D3DXVECTOR2& size, const D3DXMATRIX& transform)
{
    const D3DXVECTOR2 s(transform._11, transform._22);
    const D3DXVECTOR2 t(transform._41, transform._42);
    
    const float left = VPos.x * s.x + t.x;
    const float top  = VPos.y * s.y + t.y;
    const float right  = left + size.x * s.x;
    const float bottom = top  + size.y * s.y;
    
    ::SetRect(&m_rect, 
              static_cast<int>(left),
              static_cast<int>(top),
              static_cast<int>(right),
              static_cast<int>(bottom));
}

void UIObject::SetViewportPosRect(const D3DXVECTOR3& parentViewportPos, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DXMATRIX& transform)
{
    SetViewportPosition(parentViewportPos, pos);
    SetRect(m_viewportPosition, size, transform);
}

void UIObject::Update(const D3DXVECTOR3& parentViewportPos, const D3DXMATRIX& transform)
{
    SetViewportPosRect(parentViewportPos, m_position, m_size, transform);
    UpdateChildren(m_viewportPosition, transform);
}

void UIObject::Render()
{
    Sprite()()->SetTransform(&m_transform);

    DrawBorder();
    RenderChildren();
}

void UIObject::UpdateChildren(const D3DXVECTOR3& viewportPos, const D3DXMATRIX& transform)
{
    for (auto c : m_children)
        if (c) c->Update(viewportPos, transform);
}

UIObject* UIObject::GetChild(const int key)
{
    if (static_cast<size_t>(key) >= m_children.size()) return nullptr;

    return m_children[key];
}

void UIObject::RenderChildren()
{
    for (auto c : m_children)
        if (c) c->Render();
}

void UIObject::ClearChildren()
{
    for (auto& c : m_children)
        SAFE_DELETE(c);

    m_children.resize(0);
}

void UIObject::SetInstanceID(const int val)
{
    m_instanceID = val;
}

int UIObject::GetInstanceID() const
{
    return m_instanceID;
}

void UIObject::AddChild(UIObject* pChild)
{
    m_children.emplace_back(pChild);
}

void UIObject::SetPosition(const D3DXVECTOR3& val)
{
    m_position = val;
}

const D3DXVECTOR3& UIObject::GetPosition() const
{
    return m_position;
}

void UIObject::SetCenter(const D3DXVECTOR3& val)
{
    m_center = val;
}

void UIObject::SetSize(const D3DXVECTOR2& val)
{
    m_size = val;
}

const D3DXVECTOR2& UIObject::GetSize() const
{
    return m_size;
}

void UIObject::SetColor(const D3DCOLOR val)
{
    m_color = val;
}

void UIObject::AttachToObject(IObject* p)
{
    if (!p) return;

    m_pAttachedObject = p;
}

IObject* UIObject::GetAttachedObject() const
{
    return m_pAttachedObject;
}

void UIObject::SetTransform(const D3DXMATRIX& transform)
{
    m_transform = transform;
}
