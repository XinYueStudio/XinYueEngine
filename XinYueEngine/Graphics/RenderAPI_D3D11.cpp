#include "RenderAPI.h"
#include "PlatformBase.h"


#if SUPPORT_D3D11
#include "RenderAPI_D3D11.h"

RenderAPI* CreateRenderAPI_D3D11(HWND hwnd)
{
	RenderAPI_D3D11* 	m_D3D11 = new RenderAPI_D3D11();
	m_D3D11->SetHWND(hwnd);
	return m_D3D11;
}
 



#endif