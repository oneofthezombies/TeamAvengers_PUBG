#pragma once
#include "UIObject.h"
#include "TagClientOnly.h"

class UIText : public UIObject
{
private:
    LPD3DXFONT m_pFont;
    string	   m_text;
    string*    m_pText;
    DWORD	   m_drawTextFormat;

    UIText(const LPD3DXFONT font, const D3DXVECTOR2& size, 
        const D3DCOLOR color, UIObject* pParent);

public:
	UIText(const LPD3DXFONT font, const D3DXVECTOR2& size, string* pText, 
        const D3DCOLOR color, UIObject* pParent);
    UIText(const LPD3DXFONT font, const D3DXVECTOR2& size, const string& text, 
        const D3DCOLOR color, UIObject* pParent);
	virtual ~UIText();

	virtual void Render() override;

    void SetFont(const LPD3DXFONT p);
    
    void SetText(const string& val);
    void SetText(const string& val, UIText*& shadow);
    void SetText(string* p);
    string GetText();

    void SetDrawTextFormat(const DWORD val);
};

