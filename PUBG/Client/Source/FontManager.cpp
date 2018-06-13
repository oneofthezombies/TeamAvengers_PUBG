#include "stdafx.h"
#include "FontManager.h"

FontManager::FontManager()
    : Singleton<FontManager>()
{
}

FontManager::~FontManager()
{
}

LPD3DXFONT FontManager::GetFont(const TAG_FONT tag)
{
    const auto search = m_fonts.find(tag);
    HRESULT hr = -1;
    if (search == m_fonts.end())
    {
        switch (tag)
        {
        case TAG_FONT::DEFAULT:
            {
                hr = D3DXCreateFont(g_pDevice, 24, 12, FW_NORMAL, 1, false, 
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
                    FF_DONTCARE, TEXT("궁서체"), &m_fonts[tag]);
            }
            break;
        default:
            {

            }
            break;
        }
    }
    assert(!FAILED(hr) && "FontManager::GetFont() failed.");
    return m_fonts[tag];

	//if (m_fonts.find(tag) == m_fonts.end())
	//{
	//	if (val == Font::kIdle)
	//	{
	//		D3DXCreateFont(g_pDevice, 24, 12, FW_NORMAL, 1, false,
 //                          DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
 //                          TEXT("궁서체"), &m_umapFont[val]);
	//	}
	//	else if (val == Font::kQuest)
	//	{
	//		AddFontResource(_T("resources/fonts/umberto.tff"));
	//		D3DXCreateFont(g_pDevice, 24, 12, FW_NORMAL, 1, false,
 //                          DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
 //                          TEXT("umberto"), &m_umapFont[val]);
	//	}
 //       else if (val == Font::kGameOverDescription)
 //       {
 //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
 //           D3DXCreateFont(g_pDevice, 36, 18, FW_NORMAL, 1, false,
 //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 
 //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
 //       }
 //       else if (val == Font::kGameOverRanking)
 //       {
 //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
 //           D3DXCreateFont(g_pDevice, 60, 30, FW_NORMAL, 1, false,
 //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
 //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
 //       }
 //       else if (val == Font::kGameOverRankingNumOther)
 //       {
 //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
 //           D3DXCreateFont(g_pDevice, 48, 24, FW_NORMAL, 1, false,
 //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
 //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
 //       }
 //       else if (val == Font::kGameOverNickname)
 //       {
 //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
 //           D3DXCreateFont(g_pDevice, 48, 24, FW_NORMAL, 1, false,
 //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
 //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
 //       }
 //       else if (val == Font::kGameOverGoToLobby)
 //       {
 //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
 //           D3DXCreateFont(g_pDevice, 36, 18, FW_NORMAL, 1, false,
 //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
 //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
 //       }
 //       else if (val == Font::kInteractionMessageF)
 //       {
 //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
 //           D3DXCreateFont(g_pDevice, 16, 8, FW_NORMAL, 1, false,
 //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
 //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
 //       }
 //       else if (val == Font::kInteractionMessageDescription)
 //       {
 //           AddFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
 //           D3DXCreateFont(g_pDevice, 18, 9, FW_NORMAL, 1, false,
 //                          HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
 //                          TEXT("08서울남산체 M"), &m_umapFont[val]);
 //       }
	//}
	//return m_umapFont[val];
}

void FontManager::Destroy()
{
	for (auto f : m_fonts)
		SAFE_RELEASE(f.second);

    //RemoveFontResource(TEXT("resources/fonts/SeoulNamsanM.ttf"));
    //RemoveFontResource(_T("resources/fonts/umberto.tff"));
}
