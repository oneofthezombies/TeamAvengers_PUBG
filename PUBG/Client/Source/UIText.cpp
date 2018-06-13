#include "stdafx.h"
#include "UIText.h"
#include "UIManager.h"

UIText::UIText()
    : UIObject()
    , m_pFont(nullptr)
    , m_drawTextFormat(DT_CENTER | DT_VCENTER)
    , m_text("")
    , m_pText(nullptr)
{
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
