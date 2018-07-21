#include "stdafx.h"
#include "UsefulClientOnly.h"

POINT Mouse::GetPosition()
{
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(g_hWnd, &mouse);
    return mouse;
}
