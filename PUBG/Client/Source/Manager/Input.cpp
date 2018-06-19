#include "stdafx.h"
#include "Input.h"

char Input::GetChar()
{
    if (m_chars.empty()) return -1;

    const auto c = m_chars.front();
    m_chars.pop_front();
    return c;
}

Input::Input()
	: Singleton<Input>()
{
}

Input::~Input()
{
}

void Input::Init()
{
    //키 값을 전부 눌려져 있지 않은 상태로 초기화
    for (int i = 0; i < m_kKeyMax; ++i)
    {
        m_keyUp.set(i, false);
        m_keyDown.set(i, false);
    }
}

void Input::Update()
{
    m_prevMousePos = m_currMousePos;
    GetCursorPos(&m_currMousePos);
    ScreenToClient(g_hWnd, &m_currMousePos);

    m_bPrevIsKeyDownMouseL = m_bIsKeyDownMouseL;
    m_bPrevIsKeyDownMouseR = m_bIsKeyDownMouseR;
    m_bIsKeyDownMouseL = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    m_bIsKeyDownMouseR = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
}

void Input::Destroy()
{
}

void Input::WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
    switch (message)
    {
    case WM_CHAR:
        {
            m_chars.emplace_back(static_cast<char>(wParam));
        }
        break;
    }
}

bool Input::IsOnceKeyDown(int key)  //키를 한번만 눌렀는지
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        if (m_keyDown[key])
        {
            /* Do nothing */
        }
        else //m_keyDown[key] == false
        {
            m_keyDown.set(key, true);
            return true;
        }
    }
    else
    {
        m_keyDown.set(key, false);
    }
    return false;
}

bool Input::IsOnceKeyUp(int key)    //키를 한번 눌렀다가 뗐는지
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        m_keyUp.set(key, true);
    }
    else
    {
        if (m_keyUp[key])
        {
            m_keyUp.set(key, false);
            return true;
        }
        else
        {
            /* Do nothing */
        }
    }
    return false;
}

bool Input::IsStayKeyDown(int key)  //키가 계속 눌려져 있는지
{
    if (GetAsyncKeyState(key) & 0x8000)
        return true;
    return false;
}

bool Input::IsToggleKey(int key)    //토글키(캡스락, 넘버락)가 On상태인지
{
    if (GetKeyState(key) & 0x0001)
        return true;
    return false;
}

const POINT& Input::GetCurrentMousePos() const
{
    return m_currMousePos;
}

const POINT& Input::GetPreviousMousePos() const
{
    return m_prevMousePos;
}

bool Input::GetPrevIsKeyDownMouseL() const
{
    return m_bPrevIsKeyDownMouseL;
}

bool Input::GetPrevIsKeyDownMouseR() const
{
    return m_bPrevIsKeyDownMouseR;
}

bool Input::IsKeyDownMouseL() const
{
    return m_bIsKeyDownMouseL;
}

bool Input::IsKeyDownMouseR() const
{
    return m_bIsKeyDownMouseR;
}
