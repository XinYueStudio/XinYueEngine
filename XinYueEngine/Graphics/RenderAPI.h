#pragma once
 
#include <Resources.h>

 
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
 
	virtual	void Init(HWND hwnd, Size resolution, bool stereo)=0;
	virtual	void LoadAssets()=0;
	virtual	void Resize() = 0;
	virtual	void Update() = 0;
	virtual	void Render(int eyeindex) = 0;
	virtual	void Present() = 0;
	virtual	void Destroy() = 0;
	
	// Cached renderer properties.
	bool                                           m_WarpDevice;
	HWND										  m_Hwnd;
	D3D_FEATURE_LEVEL                m_FeatureLevel;
	bool                                           m_StereoEnabled;
	static const UINT						  m_FrameCount = 2;
	Size											  m_Resolution;
	Size											  m_RenderTargetSize;
};

#if defined(_WINDOWS)
// Create a graphics API implementation instance for the given API type.
RenderAPI* CreateRenderAPI(HWND hwnd, Size screensize, bool stereo,XinYueGfxRenderer apiType);
#endif
