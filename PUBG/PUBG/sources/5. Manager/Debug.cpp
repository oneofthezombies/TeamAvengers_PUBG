#include "stdafx.h"
#include "Debug.h"

DebugManager::DebugManager()
{
	D3DXCreateFont(g_pDevice, 12, 6, FW_NORMAL, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("³ª´®°íµñ"), &m_pFont);
}

DebugManager::~DebugManager()
{
}

void DebugManager::_AddText(CString& str)
{
    if (m_bUseRetainStr)
    {
        m_retainStr.Append(str);
    }
    else
    {
        m_str.Append(str);
    }
}

void DebugManager::Destroy()
{
	SAFE_RELEASE(m_pFont);
}

void DebugManager::InitText()
{
    m_str.SetString(_T(""));
}

void DebugManager::AddText(CString & str)
{
    _AddText((str));
}

void DebugManager::AddText(LPCTSTR str)
{
    CString _str = str;
    _AddText(_str);
}

void DebugManager::AddText(string str)
{
    CString _str = str.c_str();
    _AddText(_str);
}

void DebugManager::AddText(float str)
{
    CString _str = to_string(str).c_str();
    _AddText(_str);
}

void DebugManager::AddText(double str)
{
    CString _str = to_string(str).c_str();
    _AddText(_str);
}

void DebugManager::AddText(int str)
{
    CString _str = to_string(str).c_str();
    _AddText(_str);
}

void DebugManager::AddText(DWORD str)
{
    CString _str = to_string(str).c_str();
    _AddText(_str);
}

void DebugManager::AddText(size_t str)
{
    CString _str = to_string(str).c_str();
    _AddText(_str);
}

void DebugManager::AddText(D3DXVECTOR3 str)
{
    string tmp = string(to_string(str.x) + " , " + to_string(str.y) + " , " + to_string(str.z));
    CString _str = tmp.c_str();
    _AddText(_str);
}

void DebugManager::EndLine()
{
    AddText(_T("\n"));
}

void DebugManager::Print()
{
    m_str.Append(_T("\n"));
    m_str.Append(m_retainStr);

    RECT rc;
    SetRect(&rc, 8, 8, 8, 8);
    m_pFont->DrawText(NULL, m_str, m_str.GetLength(), &rc,
                      DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));

    if (GetAsyncKeyState('M') & 0x0001)
    {
        Debug->ShowMessageBox();
    }
}

void DebugManager::ShowMessageBox()
{
    MessageBox(NULL, (m_str), _T("DEBUG"), MB_OK);
}

void DebugManager::InitRetainStr()
{
    m_retainStr.SetString(_T(""));
}

ostream& operator<<(ostream& os, const D3DXVECTOR3& val)
{
    os << "[ " << val.x << ", " << val.y << ", " << val.z << " ]";
    return os;
}
