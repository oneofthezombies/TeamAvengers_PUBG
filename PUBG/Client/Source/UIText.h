#pragma once
#include "UIObject.h"
#include "TagFont.h"

class UIText : public UIObject
{
private:
    LPD3DXFONT m_pFont;
    string	   m_text;
    string*    m_pText;
    DWORD	   m_drawTextFormat;

public:
	UIText();
	virtual ~UIText();

	virtual void Render() override;

    void SetFont(const LPD3DXFONT p);
    void SetText(const string& val);
    void SetText(string* p);
    void SetDrawTextFormat(const DWORD val);
};

