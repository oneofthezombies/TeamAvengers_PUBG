#include "stdafx.h"
#include "Debugger.h"

Debugger::Debugger()
{
	D3DXCreateFont(g_pDevice, 12, 6, FW_NORMAL, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("³ª´®°íµñ"), &m_pFont);
}

Debugger::~Debugger()
{
}

void Debugger::Destroy()
{
	SAFE_RELEASE(m_pFont);
}

void Debugger::Print()
{
    RECT rc;
    SetRect(&rc, 8, 8, 8, 8);
	string str = m_stringstream.str();
    m_pFont->DrawText(NULL, str.c_str(), str.size(), &rc,
                      DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));

    if (GetAsyncKeyState('M') & 0x0001)
        g_pDebugger->ShowMessageBox();
}

void Debugger::Clear()
{
	m_stringstream.str("");
}

void Debugger::ShowMessageBox()
{
    MessageBox(NULL, m_stringstream.str().c_str(), _T("DEBUG"), MB_OK);
}

stringstream& Debugger::GetStringStream()
{
	return m_stringstream;
}

