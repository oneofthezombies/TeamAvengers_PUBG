#include "stdafx.h"
#include "UIText.h"
#include "UIManager.h"

UIText::UIText()
    : UIObject()
    , m_pFont(nullptr)
    , m_drawTextFormat(DT_CENTER | DT_VCENTER)
    , m_textString()
    , m_text(nullptr)
    , m_pTextString(nullptr)
{
}

UIText::~UIText()
{
}

void UIText::Render()
{
    if (!m_pFont) return;

    if (!m_textString.empty())
        m_pFont->DrawTextA(g_pSprite, m_textString.c_str(), m_textString.size(), &m_rect, m_drawTextFormat, m_color);
    else if (m_text)
	    m_pFont->DrawText(g_pSprite, m_text, lstrlen(m_text), &m_rect, m_drawTextFormat, m_color);
    else if (m_pTextString)
        m_pFont->DrawTextA(g_pSprite, m_pTextString->c_str(), m_pTextString->size(), &m_rect, m_drawTextFormat, m_color);
    
    UIObject::Render();
}

void UIText::SetFont(const LPD3DXFONT val)
{
    m_pFont = val;
}

void UIText::SetText(const LPCTSTR val)
{
    m_text = val;
}

void UIText::SetText(const string& val)
{
    m_textString = val;
}

void UIText::SetText(string* val)
{
    m_pTextString = val;
}

void UIText::SetDrawTextFormat(const DWORD val)
{
    m_drawTextFormat = val;
}

UIText* UIText::Create(const Font::Type font, const string& text, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, UIObject* parent, const DWORD format)
{
    UIText* ret = new UIText;
    ret->SetFont(g_pFontManager->GetFont(font));
    ret->SetText(text);
    ret->SetPosition(pos);
    ret->SetSize(size);
    ret->SetDrawTextFormat(format);

    if (parent)
        parent->AddChild(*ret);
    else
        g_pUIManager->RegisterUIObject(*ret);

    return ret;
}
