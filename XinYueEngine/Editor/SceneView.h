#pragma once
#include <windows.h>
#include "RenderAPI.h"
#include "PlatformBase.h"
#define SMAX_LOADSTRING 100

// 首先需要定义菜单的ID
#define IDM_FILE_NEW  40001

#define IDM_FILE_OPEN 40002

#define IDM_EDIT_COPY 40003

#define IDM_EDIT_CUT  40004

class SceneView
{
public:
	SceneView(HINSTANCE hInstance, HWND parenthwnd);
	~SceneView();

	RenderAPI* m_RenderAPI;
 
	virtual void OnUpdate();

 
	HWND hWnd;
 
 
};

