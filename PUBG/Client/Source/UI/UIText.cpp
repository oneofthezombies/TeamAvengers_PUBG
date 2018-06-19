#include "stdafx.h"
#include "UIText.h"
#include "UIManager.h"

UIText::UIText(const LPD3DXFONT font, const D3DXVECTOR2& size, const D3DCOLOR color, UIObject* pParent)
    : UIObject(pParent)
    , m_pFont(nullptr)
    , m_drawTextFormat(DT_CENTER | DT_VCENTER)
    , m_text("")
    , m_pText(nullptr)
{
    if (font)
        SetFont(font);

    SetSize(size);
    SetColor(color);
}

UIText::UIText(const LPD3DXFONT font, const D3DXVECTOR2& size, string* pText, const D3DCOLOR color, UIObject* pParent)
    : UIText(font, size, color, pParent)
{
    if (pText)
        SetText(pText);
}

UIText::UIText(const LPD3DXFONT font, const D3DXVECTOR2& size, const string& text, const D3DCOLOR color, UIObject* pParent)
    : UIText(font, size, color, pParent)
{
    SetText(text);
}

UIText::~UIText()
{
}

void UIText::Render()
{
    if (!m_pFont) return;

    if (m_pText)
    {
        m_pFont->DrawTextA(g_pSprite, m_pText->c_str(), m_pText->size(), &m_rect, m_drawTextFormat, m_color);
    }
    else
    {
        m_pFont->DrawTextA(g_pSprite, m_text.c_str(), m_text.size(), &m_rect, m_drawTextFormat, m_color);
    }
    
    UIObject::Render();
}

void UIText::SetFont(const LPD3DXFONT val)
{
    m_pFont = val;
}

void UIText::SetText(const string& val)
{
    m_text = val;
}

void UIText::SetText(string* p)
{
    if (!p) return;

    m_pText = p;
}

void UIText::SetDrawTextFormat(const DWORD val)
{
    m_drawTextFormat = val;
}
