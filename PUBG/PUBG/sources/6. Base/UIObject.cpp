#include "stdafx.h"
#include "UIObject.h"
#include "UIManager.h"

UIObject::UIObject()
    : m_instanceID(-1)
    , m_color(D3DCOLOR_XRGB(255, 255, 255))
    , m_pParent(nullptr)
    , m_vPosition()
    , m_vCenter()
    , m_vViewportPosition()
    , m_vSize()
    , m_pAttachedObject(nullptr)
{
}

UIObject::~UIObject()
{
    for (auto c : m_deqPChildren)
        SAFE_DELETE(c);
}

const D3DXVECTOR3& UIObject::GetViewportPosition() const
{
    return m_vViewportPosition;
}

void UIObject::DrawBorder()
{
    if (!g_pUIManager->IsDrawBorder()) return;

    D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255);
    vector<VERTEX_RHWC> vertices = 
    {
        VERTEX_RHWC( static_cast<float>(m_rect.left),  static_cast<float>(m_rect.top),    0, 1, c ),
        VERTEX_RHWC( static_cast<float>(m_rect.right), static_cast<float>(m_rect.top),    0, 1, c ),
        VERTEX_RHWC( static_cast<float>(m_rect.right), static_cast<float>(m_rect.bottom), 0, 1, c ),
        VERTEX_RHWC( static_cast<float>(m_rect.left),  static_cast<float>(m_rect.bottom), 0, 1, c ),
        VERTEX_RHWC( static_cast<float>(m_rect.left),  static_cast<float>(m_rect.top),    0, 1, c ),
    };
    
    const auto dv = g_pDevice;
    dv->SetFVF(VERTEX_RHWC::FVF);
    dv->DrawPrimitiveUP(D3DPT_LINESTRIP, vertices.size() - 1, vertices.data(), sizeof VERTEX_RHWC);
}

void UIObject::UpdateViewportPosRect()
{
    UpdateViewportPosition();
    UpdateRect();
}

void UIObject::UpdateViewportPosition()
{
    m_vViewportPosition = m_vPosition;

    if (m_pParent)
        m_vViewportPosition += m_pParent->GetViewportPosition();
}

void UIObject::UpdateRect()
{
    D3DXMATRIX m;
    g_pSprite->GetTransform(&m);
    
    const float left = m_vViewportPosition.x * m._11 + m._41;
    const float top  = m_vViewportPosition.y * m._22 + m._42;
    const float right  = left + m_vSize.x * m._11;
    const float bottom = top  + m_vSize.y * m._22;
    
    SetRect(&m_rect, 
            static_cast<int>(left), 
            static_cast<int>(top), 
            static_cast<int>(right), 
            static_cast<int>(bottom));
}

void UIObject::Init()
{
}

void UIObject::Release()
{
    ReleaseChildren();
}

void UIObject::Update()
{
    UpdateViewportPosRect();
    UpdateChildren();
}

void UIObject::Render()
{
    DrawBorder();
    RenderChildren();
}

void UIObject::ReleaseChildren()
{
    for (auto c : m_deqPChildren)
        SAFE_RELEASE(c);
}

void UIObject::UpdateChildren()
{
    for (auto c : m_deqPChildren)
        SAFE_UPDATE(c);
}

void UIObject::RenderChildren()
{
    for (auto c : m_deqPChildren)
        SAFE_RENDER(c);
}

void UIObject::ClearChildren()
{
    ReleaseChildren();

    for (auto& c : m_deqPChildren)
        SAFE_DELETE(c);

    m_deqPChildren.resize(0);
}

void UIObject::SetInstanceID(const int val)
{
    m_instanceID = val;
}

int UIObject::GetInstanceID() const
{
    return m_instanceID;
}

void UIObject::SetParent(UIObject& parent)
{
    m_pParent = &parent;
}

void UIObject::AddChild(UIObject& child)
{
    child.SetParent(*this);
    m_deqPChildren.emplace_back(&child);
}

void UIObject::SetPosition(const D3DXVECTOR3& val)
{
    m_vPosition = val;
}

void UIObject::SetCenter(const D3DXVECTOR3& val)
{
    m_vCenter = val;
}

void UIObject::SetSize(const D3DXVECTOR2& val)
{
    m_vSize = val;
}

const D3DXVECTOR2& UIObject::GetSize() const
{
    return m_vSize;
}

void UIObject::SetColor(const D3DCOLOR& val)
{
    m_color = val;
}

void UIObject::AttachToObject(BaseObject& val)
{
    m_pAttachedObject = &val;
}

BaseObject* UIObject::GetAttachedObject() const
{
    return m_pAttachedObject;
}

//IUIObject* IUIObject::FindChildByUITag(int uiTag)
//{
//	if (m_uiTag == uiTag)
//		return this;
//
//	for (auto p : m_vecPChild)
//	{
//        IUIObject* pChild = static_cast<IUIObject*>(p)->FindChildByUITag(uiTag);
//
//		if (pChild)
//			return pChild;
//	}
//
//	return nullptr;
//}