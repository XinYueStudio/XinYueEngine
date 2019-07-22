#include "GraphicRender.h"
#if SUPPORT_D3D11
void GraphicRender::Init(HWND hwnd, Size screensize, bool stereo)
{
	RenderAPI_D3D11::Init(hwnd, screensize, stereo);
}

void GraphicRender::LoadAssets()
{
	RenderAPI_D3D11::LoadAssets();
}

void GraphicRender::Resize()
{
	RenderAPI_D3D11::Resize();
}

void GraphicRender::Update()
{
	RenderAPI_D3D11::Update();
}

void GraphicRender::Render()
{
	RenderAPI_D3D11::Render();
}

void GraphicRender::Present()
{
	RenderAPI_D3D11::Present();
}

void GraphicRender::Destroy()
{
	RenderAPI_D3D11::Destroy();
}
#endif