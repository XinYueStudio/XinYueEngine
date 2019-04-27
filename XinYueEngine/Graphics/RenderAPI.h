#pragma once
 
 
#include <stddef.h>

#if defined(_WINDOWS)
// Windows Í·ÎÄ¼þ
#include <windows.h>

#endif

// XinYue native plugin API
// Compatible with C99

#if defined(__CYGWIN32__)
#define XINYUE_INTERFACE_API __stdcall
#define XINYUE_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#define XINYUE_INTERFACE_API __stdcall
#define XINYUE_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__)
#define XINYUE_INTERFACE_API
#define XINYUE_INTERFACE_EXPORT
#else
#define XINYUE_INTERFACE_API
#define XINYUE_INTERFACE_EXPORT
#endif


typedef enum XinYueGfxRenderer
{
	
	kXinYueGfxRendererD3D11 = 0, // Direct3D 11
	kXinYueGfxRendererD3D12 = 1,// Direct3D 12
	kXinYueGfxRendererOpenGL = 2, //  OpenGL
} XinYueGfxRenderer;

typedef enum XinYueGfxDeviceEventType
{
	kXinYueGfxDeviceEventInitialize = 0,
	kXinYueGfxDeviceEventShutdown = 1,
	kXinYueGfxDeviceEventBeforeReset = 2,
	kXinYueGfxDeviceEventAfterReset = 3,

} XinYueGfxDeviceEventType;

struct Resolution
{
	UINT w;
	UINT h;

 
};


// Super-simple "graphics abstraction". This is nothing like how a proper platform abstraction layer would look like;
// all this does is a base interface for whatever our plugin sample needs. Which is only "draw some triangles"
// and "modify a texture" at this point.
//
// There are implementations of this base class for D3D9 D3D12, OpenGL etc.; see individual RenderAPI_* files.
class RenderAPI
{
public:
	 
	virtual ~RenderAPI() { }
 
	virtual void* GetRenderDevice() = 0;
 
	virtual void OnStart() = 0;

	virtual void OnUpdate() = 0;

	virtual void OnDestroy() = 0;

	virtual void OnKeyDown(UINT8 /*key*/) {}

	virtual void OnKeyUp(UINT8 /*key*/) {}


	static const UINT FrameCount = 2;
	// Viewport dimensions.
	Resolution m_Resolution;
	 
	bool	m_WarpDevice;
};

#if defined(_WINDOWS)
// Create a graphics API implementation instance for the given API type.
RenderAPI* CreateRenderAPI(HWND hwnd,XinYueGfxRenderer apiType);
#endif
