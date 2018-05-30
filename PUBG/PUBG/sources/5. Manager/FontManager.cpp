#include "stdafx.h"
#include "FontManager.h"

FontManager::FontManager()
    : SingletonBase<FontManager>()
{
}

FontManager::~FontManager()
{
}

LPD3DXFONT FontManager::GetFont(const Font::Type val)
{
    //없는 상태이면
	if (m_umapFont.find(val) == m_umapFont.end())
	{
		if (val == Font::kIdle)
		{
			D3DXCreateFont(g_pDevice, 24, 12, FW_NORMAL, 1, false,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
                           TEXT("궁서체"), &m_umapFont[val]);
		}
		else if (val == Font::kQuest)
		{
			AddFontResource(_T("resources/fonts/umberto.tff"));
			D3DXCreateFont(g_pDevice, 24, 12, FW_NORMAL, 1, false,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
                           TEXT("umberto"), &m_umapFont[val]);
		}
        else if (val == Font::kGameOverDescription)
        {
            AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
            D3DXCreateFont(g_pDevice, 36, 18, FW_NORMAL, 1, false,
                           HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
                           TEXT("08서울남산체 M"), &m_umapFont[val]);
        }
        else if (val == Font::kGameOverRanking)
        {
            AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
            D3DXCreateFont(g_pDevice, 60, 30, FW_NORMAL, 1, false,
                           HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
                           TEXT("08서울남산체 M"), &m_umapFont[val]);
        }
        else if (val == Font::kGameOverRankingNumOther)
        {
            AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
            D3DXCreateFont(g_pDevice, 48, 24, FW_NORMAL, 1, false,
                           HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
                           TEXT("08서울남산체 M"), &m_umapFont[val]);
        }
        else if (val == Font::kGameOverNickname)
        {
            AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
            D3DXCreateFont(g_pDevice, 48, 24, FW_NORMAL, 1, false,
                           HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
                           TEXT("08서울남산체 M"), &m_umapFont[val]);
        }
        else if (val == Font::kGameOverGoToLobby)
        {
            AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
            D3DXCreateFont(g_pDevice, 36, 18, FW_NORMAL, 1, false,
                           HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
                           TEXT("08서울남산체 M"), &m_umapFont[val]);
        }
        else if (val == Font::kInteractionMessageF)
        {
            AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
            D3DXCreateFont(g_pDevice, 16, 8, FW_NORMAL, 1, false,
                           HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
                           TEXT("08서울남산체 M"), &m_umapFont[val]);
        }
        else if (val == Font::kInteractionMessageDescription)
        {
            AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
            D3DXCreateFont(g_pDevice, 18, 9, FW_NORMAL, 1, false,
                           HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
                           TEXT("08서울남산체 M"), &m_umapFont[val]);
        }
	}
	return m_umapFont[val];
}

void FontManager::Destroy()
{
	for (auto p : m_umapFont)
		SAFE_RELEASE(p.second);

    RemoveFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    RemoveFontResource(_T("resources/fonts/umberto.tff"));

    m_umapFont.clear();
}
