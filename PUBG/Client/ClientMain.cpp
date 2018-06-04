#include "stdafx.h"
#include "Client.h"

//#define MAX_LOADSTRING 100
//
//HINSTANCE hInst;
//CHAR szTitle[MAX_LOADSTRING];
//CHAR szWindowClass[MAX_LOADSTRING];
//HWND g_hWnd;
//
//ATOM             MyRegisterClass(HINSTANCE hInstance);
//BOOL             InitInstance(HINSTANCE, int);
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY WinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    strcpy_s(szTitle, "TeamAvengers_PUBG");
//    strcpy_s(szWindowClass, "TeamAvengers_PUBG");
//
//    MyRegisterClass(hInstance);
//    if (!InitInstance(hInstance, nCmdShow))
//        return FALSE;
//
//    //g_pApp->Init();
//
//    MSG msg;
//    while (true)
//    {
//        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
//        {
//            if (msg.message == WM_QUIT)
//                break;
//
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//        else
//        {
//            //g_pApp->Update();
//            //g_pApp->Render();
//        }
//    }
//
//    //g_pApp->Destroy();
//
//    return (int)msg.wParam;
//}
//
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASS wc;
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpfnWndProc = WndProc;
//    wc.cbClsExtra = 0;
//    wc.cbWndExtra = 0;
//    wc.hInstance = hInstance;
//    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wc.lpszMenuName = NULL;
//    wc.lpszClassName = szWindowClass;
//
//    return RegisterClass(&wc);
//}
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
//
//    const int style = WS_OVERLAPPED | WS_CAPTION | WS_DLGFRAME | WS_SYSMENU;
//    const int left = 500;
//    const int top = 0;
//    const int width = 1280;
//    const int height = 720;
//    HWND hWnd = CreateWindow(szWindowClass, szTitle, style, left, top, width, height, nullptr, nullptr, hInstance, nullptr);
//
//    g_hWnd = hWnd;
//
//    if (!hWnd)
//        return FALSE;
//
//    RECT rc{ 0, 0, width, height };
//    AdjustWindowRect(&rc, style, false);
//    SetWindowPos(hWnd, NULL, left, top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
//    ShowWindow(hWnd, nCmdShow);
//    UpdateWindow(hWnd);
//
//    return TRUE;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    //g_pApp->WndProc(hWnd, message, wParam, lParam);
//
//    //Alt키 막기용
//    if (wParam == SC_KEYMENU && (lParam >> 16) <= 0)
//        return 0;
//
//    switch (message)
//    {
//    case WM_KEYDOWN:
//        {
//            switch (wParam)
//            {
//            case VK_ESCAPE:
//                {
//                    SendMessage(hWnd, WM_DESTROY, 0, 0);
//                }
//                break;
//            }
//        }
//        break;
//    case WM_DESTROY:
//        {
//            PostQuitMessage(0);
//        }
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}

int main(int argc, char* argv[])
{
    try 
    {
        if (argc != 3)
        {
            std::cerr << "Usage: Client <host> <port>\n";
            return 1;
        }

        boost::asio::io_context ioContext;
        tcp::resolver resolver(ioContext);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        Client c(ioContext, endpoints);
        std::thread t([&ioContext]() { ioContext.run(); });

        while (1)
        {
            Message msg;

            // write
        }

        c.Close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}