#include "stdafx.h"
#include "DebugManager.h"

DebugManager::DebugManager()
    : Singleton<DebugManager>()
    , m_pFont(nullptr)
    , m_isRender(false)
    , m_hasDebugSpeed(false)
    , m_isHoonsComputer(false)
{
}

DebugManager::~DebugManager()
{
}

void DebugManager::Init(const bool isHoonsComputer)
{
    m_isHoonsComputer = isHoonsComputer;
}

void DebugManager::Destroy()
{
    SAFE_RELEASE(m_pFont);
}

void DebugManager::Update()
{
    if (!m_pFont)
    {
        D3DXCreateFontA(Device()(), 12, 6, FW_NORMAL, 1, false, HANGEUL_CHARSET,
            OUT_OUTLINE_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "����ü", &m_pFont);
    }

    if (GetAsyncKeyState('0') & 0x0001)
        m_hasDebugSpeed = !m_hasDebugSpeed;
    
    Debug << "Key 0 : ";

    if (m_hasDebugSpeed)
        Debug << "Debug Moving Speed On\n";
    else
        Debug << "Debug Moving Speed Off\n";
}

void DebugManager::Render()
{
    if (GetAsyncKeyState('M') & 0x0001)
        m_isRender = !m_isRender;

    if (GetAsyncKeyState('N') & 0x0001)
    {
        bool isRender = Collision()()->IsRender();
        Collision()()->SetIsRender(!isRender);
    }
        

    if (!m_isRender) return;

	const std::string str = m_stringstream.str();
    std::size_t numNewLine = 0;
    std::vector<std::string> newLinedStrs;
    std::string newLinedStr;
    for (std::size_t i = 0; i < str.size(); ++i)
    {
        newLinedStr.push_back(str[i]);

        if (str[i] == '\n')
            ++numNewLine;

        if (numNewLine == 50)
        {
            newLinedStrs.emplace_back(newLinedStr);
            newLinedStr = "";
            numNewLine = 0;
        }
    }
    newLinedStrs.emplace_back(newLinedStr);

    RECT rc;
    for (std::size_t i = 0; i < newLinedStrs.size(); ++i)
    {
        const int x = static_cast<int>(i * 630) + 8;

        SetRect(&rc, x, 8, x, 8);
        if (!m_pFont) continue;

        m_pFont->DrawTextA(
            nullptr, 
            newLinedStrs[i].c_str(), 
            static_cast<INT>(newLinedStrs[i].size()), 
            &rc,
            DT_LEFT | DT_TOP | DT_NOCLIP, 
            D3DCOLOR_XRGB(255, 0, 255));
    }
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

bool DebugManager::HasDebugSpeed() const
{
    return m_hasDebugSpeed;
}

bool DebugManager::IsHoonsComputer() const
{
    return m_isHoonsComputer;
}

