#pragma once

#include "SingletonBase.h"

#define g_pFontManager FontManager::GetInstance()

struct Font
{
    enum Type
    {
        kIdle,
        kQuest,
        kGameOverDescription,
        kGameOverRanking,
        kGameOverRankingNumOther,
        kGameOverNickname,
        kGameOverGoToLobby,
        kInteractionMessageF,
        kInteractionMessageDescription
    };
};

class FontManager
    : public SingletonBase<FontManager>
{
private:
	unordered_map<Font::Type, LPD3DXFONT> m_umapFont;

    FontManager();
    virtual ~FontManager();

public:
	LPD3DXFONT GetFont(const Font::Type val);
	void Destroy();

    friend SingletonBase<FontManager>;
};

