#pragma once
#include "Resources.h"

class GraphicRender
{
public:
	virtual	void Init(HWND hwnd, Size screensize, bool stereo);
	virtual	void LoadAssets();
	virtual	void Resize();
	virtual	void Update();
	virtual	void Render();
	virtual	void Present();
	virtual	void Destroy();


	HWND	m_Hwnd;

	// Pipeline objects.	
	// Direct3D 11.1
	ComPtr < ID3D11Device2> m_Device;                  // D3D11.1设备
	ComPtr < ID3D11DeviceContext2> m_DeviceContext; // D3D11.1设备上下文
	ComPtr < IDXGISwapChain1> m_SwapChain;                // D3D11.1交换链

	ComPtr < ID3D11RenderTargetView> m_RenderTargetView;
	ComPtr < ID3D11RenderTargetView> m_RenderTargetViewRight;
	ComPtr < ID3D11DepthStencilView>  m_DepthStencilView;


	// Cached renderer properties.
	D3D_FEATURE_LEVEL                m_featureLevel;
	bool                                           m_stereoEnabled;
	static const UINT						  FrameCount = 2;
	Size											  m_Resolution;
	Size											  m_renderTargetSize;
};