#pragma once
#include "Singleton.h"

#define g_pDebugMgr DebugMgr()()
#define Debug       DebugMgr()()->GetStringStream()

class DebugManager : public Singleton<DebugManager>
{
private:
	LPD3DXFONT	      m_pFont;
	std::stringstream m_stringstream;
    bool              m_isRender;
    bool              m_hasDebugSpeed;

             DebugManager();
	virtual ~DebugManager();

public:
	void Destroy();
    void Update();
	void Render();
	void Clear();
	void ShowMessageBox();

	stringstream& GetStringStream();

    bool HasDebugSpeed() const;

	friend Singleton<DebugManager>;
};

struct DebugMgr
{
    DebugManager* operator()()
    {
        return DebugManager::GetInstance();
    }
};