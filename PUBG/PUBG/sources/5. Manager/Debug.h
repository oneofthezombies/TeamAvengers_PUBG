#pragma once

#define Debug DebugManager::GetInstance()

class DebugManager
{
	SINGLETON(DebugManager)
private:
	LPD3DXFONT			m_pFont;
    CString		m_str;
    CString		m_retainStr;
    bool		m_bUseRetainStr;

    void _AddText(CString& str);

public:
	void Destroy();
	void InitText();
    void AddText(CString& str);
    void AddText(LPCTSTR str);
	void AddText(string str);
	void AddText(float str);
	void AddText(double str);
	void AddText(int str);
	void AddText(DWORD str);
    void AddText(size_t str);
	void AddText(D3DXVECTOR3 str);
	void EndLine();
	void Print();
	void ShowMessageBox();
    void InitRetainStr();
    void UsingRetainStr(bool bUse = true) { m_bUseRetainStr = bUse; }
};

ostream& operator<<(ostream& os, const D3DXVECTOR3& val);