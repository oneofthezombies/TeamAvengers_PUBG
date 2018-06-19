#pragma once
#include "Singleton.h"

#define g_pDebugger Debugger::GetInstance()
#define Debug       Debugger::GetInstance()->GetStringStream()

class Debugger : public Singleton<Debugger>
{
private:
	LPD3DXFONT	 m_pFont;
	stringstream m_stringstream;

	Debugger();
	virtual ~Debugger();

public:
	void Destroy();
	void Print();
	void Clear();
	void ShowMessageBox();

	stringstream& GetStringStream();

	friend Singleton<Debugger>;
};

