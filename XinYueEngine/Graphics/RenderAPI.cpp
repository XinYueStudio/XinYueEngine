#include "RenderAPI.h"
#include "PlatformBase.h"

#if defined(_WINDOWS)

RenderAPI* CreateRenderAPI(HWND hwnd, Size screensize, bool stereo, XinYueGfxRenderer apiType)
{

	switch (apiType)
	{
	case kXinYueGfxRendererD3D11:
	{
#	if SUPPORT_D3D11
		extern RenderAPI* CreateRenderAPI_D3D11(HWND hwnd, Size screensize, bool stereo);
		return CreateRenderAPI_D3D11(hwnd,  screensize, stereo);
#endif  
	}
	 
	case kXinYueGfxRendererD3D12:
	{
#	if SUPPORT_D3D12
		extern RenderAPI* CreateRenderAPI_D3D12(HWND hwnd, Size screensize, bool stereo);
		return CreateRenderAPI_D3D12(hwnd , screensize, stereo);
#endif  
	}
	 
	case kXinYueGfxRendererOpenGL:
		break;
	default:
		break;
	}
 
 
 
	// Unknown or unsupported graphics API
	return NULL;
}




#endif  

 