#include "SceneView.h"
 
SceneView::SceneView(HINSTANCE hInstance,HWND parenthwnd)
{
 
	HMENU    hMenu;
	HMENU	  hMenuPop;

	// Menu Append
	hMenu = CreateMenu();		//主菜单,水平
	hMenuPop = CreateMenu();	//下拉的菜单,垂直

	AppendMenu(hMenuPop, MF_STRING, IDM_FILE_NEW, TEXT("New"));
	AppendMenu(hMenuPop, MF_STRING, IDM_FILE_OPEN, TEXT("Open"));
	AppendMenu(hMenu, MF_POPUP, (unsigned int)hMenuPop, TEXT("File"));

	hMenuPop = CreateMenu();

	AppendMenu(hMenuPop, MF_STRING, IDM_EDIT_COPY, TEXT("Copy"));
	AppendMenu(hMenuPop, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenuPop, MF_STRING, IDM_EDIT_CUT, TEXT("Cut"));
	AppendMenu(hMenu, MF_POPUP, (unsigned int)hMenuPop, TEXT("Edit"));// Menu Insert
	InsertMenu(hMenu, 0, MF_BYCOMMAND, IDM_EDIT_CUT + 7, TEXT("NewAdd"));
 
	hWnd=CreateWindow(TEXT("XinYueEngine"), TEXT("SceneView DX12..........................................."),
		 WS_OVERLAPPEDWINDOW  | WS_VISIBLE | WS_POPUP| WS_CLIPCHILDREN, 
	 CW_USEDEFAULT,
	CW_USEDEFAULT,
		640, 480,
		parenthwnd,
		hMenu,
		hInstance, NULL);

	if (!hWnd)
	{
	
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	m_RenderAPI = CreateRenderAPI(hWnd, XinYueGfxRenderer::kXinYueGfxRendererD3D12);
	Resolution mResolution = Resolution();
	mResolution.w = 1920;
	mResolution.h = 1080;
	m_RenderAPI->m_Resolution= mResolution;
	m_RenderAPI->m_WarpDevice = false;

	m_RenderAPI->OnStart(); 
 
}
void SceneView::OnUpdate()
{

 
	if(m_RenderAPI!=NULL)
	m_RenderAPI->OnUpdate();

 
 
 
 }

SceneView::~SceneView()
{


}
