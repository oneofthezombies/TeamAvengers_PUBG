#pragma once
#include "Singleton.h"

#define Debug DebugManager::GetInstance()->GetStringStream()

class DebugManager : public Singleton<DebugManager>
{
private:
	LPD3DXFONT	 m_pFont;
	stringstream m_stringstream;

             DebugManager();
	virtual ~DebugManager();

public:
	void Destroy();
	void Print();
	void Clear();
	void ShowMessageBox();

	stringstream& GetStringStream();

	friend Singleton<DebugManager>;
};

struct DebugMgr
{
    DebugManager* operator()()
    {
        return DebugManager::GetInstance();
    }
};