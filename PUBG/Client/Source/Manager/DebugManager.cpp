#include "stdafx.h"
#include "DebugManager.h"

DebugManager::DebugManager()
{
	D3DXCreateFontA(Device()(), 16, 8, FW_NORMAL, 1, false, HANGEUL_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "¸¼Àº °íµñ", &m_pFont);
}

DebugManager::~DebugManager()
{
}

void DebugManager::Destroy()
{
    SAFE_RELEASE(m_pFont);
}

void DebugManager::Print()
{
    RECT rc;
    SetRect(&rc, 8, 8, 8, 8);
	string str = m_stringstream.str();
    m_pFont->DrawTextA(nullptr, str.c_str(), str.size(), &rc,
        DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));

    if (GetAsyncKeyState('M') & 0x0001)
        ShowMessageBox();
}

void DebugManager::Clear()
{
	m_stringstream.str("");
}

void DebugManager::ShowMessageBox()
{
    MessageBoxA(nullptr, m_stringstream.str().c_str(), "DEBUG", MB_OK);
}

stringstream& DebugManager::GetStringStream()
{
	return m_stringstream;
}

