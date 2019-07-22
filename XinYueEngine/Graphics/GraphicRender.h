#pragma once
#include "RenderAPI.h"
#include "PlatformBase.h"

#if SUPPORT_D3D11
#include "RenderAPI_D3D11.h"

class GraphicRender:RenderAPI_D3D11
{
public:
	virtual	void Init(HWND hwnd, Size screensize, bool stereo);
	virtual	void LoadAssets();
	virtual	void Resize();
	virtual	void Update();
	virtual	void Render();
	virtual	void Present();
	virtual	void Destroy();
};

#endif