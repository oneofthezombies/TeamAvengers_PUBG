#pragma once
#include "UIObject.h"

class UIText : public UIObject
{
private:
    LPD3DXFONT m_pFont;
    string	   m_textString;
    string*    m_pTextString;
    LPCTSTR    m_text;
    DWORD	   m_drawTextFormat;

public:
	UIText();
	virtual ~UIText();

	virtual void Render() override;

    void SetFont(const LPD3DXFONT val);
    void SetText(const LPCTSTR val);
    void SetText(const string& val);
    void SetText(string* val);
    void SetDrawTextFormat(const DWORD val);

    static UIText* Create(const Font::Type font, const string& text, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, UIObject* parent = nullptr, const DWORD format = DT_CENTER | DT_VCENTER);
};

