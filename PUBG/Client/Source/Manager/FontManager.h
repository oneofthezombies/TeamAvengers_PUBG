#pragma once
#include "Singleton.h"
#include "TagFont.h"

#define g_pFontManager FontManager::GetInstance()

class FontManager : public Singleton<FontManager>
{
private:
	unordered_map<TAG_FONT, LPD3DXFONT> m_fonts;

    FontManager();
    virtual ~FontManager();

public:
	LPD3DXFONT GetFont(const TAG_FONT tag);
	void Destroy();

    friend Singleton<FontManager>;
};

