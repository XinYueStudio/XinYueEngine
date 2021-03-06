﻿// XinYueEngine.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "XinYueEngine.h"
#include "SceneView.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND ParenthWnd;
HWND hWnd;
//SceneView* m_SceneView;
RenderAPI* m_RenderAPI;

bool FullScreen = false;
RECT* WindowDefaultLPRECT;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_XINYUEENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_XINYUEENGINE));

 


	//m_SceneView =  new SceneView(hInstance,hWnd);
	Size mResolution = Size();
	mResolution.Width = 1920;
	mResolution.Height = 1080;
	
	m_RenderAPI = CreateRenderAPI(hWnd, mResolution,true, XinYueGfxRenderer::kXinYueGfxRendererD3D11);
	m_RenderAPI->m_Resolution = mResolution;
	m_RenderAPI->m_WarpDevice = false;
 
    MSG msg;

	//m_RenderAPI->SetFullScreen();

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
 
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (m_RenderAPI != nullptr)
			m_RenderAPI->Update();
		if (m_RenderAPI != nullptr)
			m_RenderAPI->Render(0);
		if (m_RenderAPI != nullptr)
			m_RenderAPI->Render(1);
		if (m_RenderAPI != nullptr)
			m_RenderAPI->Present();
    }
	
	m_RenderAPI->Destroy();
    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_XINYUEENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_XINYUEENGINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);

}


//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中
    /*  ParenthWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ,
	   0, 0, 1920, 1080, nullptr, nullptr, hInstance, nullptr);*/

     hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		 0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);



   //if (!ParenthWnd)
   //{
   //   return FALSE;
   //}

   //ShowWindow(ParenthWnd, nCmdShow);
   //UpdateWindow(ParenthWnd);

 
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
 
   //RECT rc; //temporary rectangle
   //GetClientRect(ParenthWnd, &rc); //the "inside border" rectangle for a
   //MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top ,true); //place b at (x,y,w,h) in a
 
 
   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 		
    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
		

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            //PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hWnd, &ps);
            //// TODO: 在此处添加使用 hdc 的任何绘图代码...
            //EndPaint(hWnd, &ps);
			
			 

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;

		case VK_LEFT:

			// Process the LEFT ARROW key. 

			break;

		case VK_RIGHT:

			// Process the RIGHT ARROW key. 

			break;

		case VK_UP:

			// Process the UP ARROW key. 

			break;

		case VK_DOWN:

			// Process the DOWN ARROW key. 

			break;

		case VK_HOME:

			// Process the HOME key. 

			break;

		case VK_END:

			// Process the END key. 

			break;

		case VK_INSERT:

			// Process the INS key. 

			break;

		case VK_DELETE:

			// Process the DEL key. 

			break;

		case VK_F2:

			 
			break;
		case VK_F11:// Process the F11 key. 
			if (FullScreen)
			{
				FullScreen = false;
				SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				if(WindowDefaultLPRECT)
				SetWindowPos(hWnd, 
					HWND_TOPMOST, 
					WindowDefaultLPRECT->left, 
					WindowDefaultLPRECT->top,
					WindowDefaultLPRECT->right- WindowDefaultLPRECT->left,
					WindowDefaultLPRECT->bottom- WindowDefaultLPRECT->top,
					SWP_SHOWWINDOW);
			}
			else
			{
				WindowDefaultLPRECT = new RECT();
				FullScreen = true;
				GetWindowRect(hWnd,WindowDefaultLPRECT);
				HWND   hDesk;
				RECT   rc;
				hDesk = GetDesktopWindow();
				GetWindowRect(hDesk, &rc);
				SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);
			}
			break;

			// Process other non-character keystrokes. 

		default:
			break;
		}
	
	 
		break;
	case WM_KEYUP:
		 
		break;
	case	WM_SIZE:
		if (m_RenderAPI != nullptr)
		{
			m_RenderAPI->Resize();
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
