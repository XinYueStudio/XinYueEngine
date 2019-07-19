#pragma once
#include "Resources.h"

class HoloGraphicRenderAPI
{

private:


	virtual	void UpdateStereoEnabledStatus();

	virtual	void UpdateForWindowSizeChange();

	virtual void HandleDeviceLost();

	virtual void CreateDeviceResources();

	virtual void CreateWindowSizeDependentResources();

	virtual void SetDpi(float dpi);

	virtual void LoadPipeline();

	virtual void LoadAssets();

	virtual void PopulateCommandList();

	virtual 	void WaitForPreviousFrame();

	virtual void CreateVertexBuffer(
		_In_ unsigned int numVertices,
		_In_ BasicVertex *vertexData,
		_Out_ ID3D11Buffer **vertexBuffer
	);

	virtual void CreateIndexBuffer(
		_In_ unsigned int numIndices,
		_In_ unsigned short *indexData,
		_Out_ ID3D11Buffer **indexBuffer
	);

	virtual void CreatePlane(
		ID3D11Buffer ** vertexBuffer,
		ID3D11Buffer ** indexBuffer,
		unsigned int * vertexCount,
		unsigned int * indexCount,
		float	frustumWidth,
		float	frustumHeight);

	bool InitializeShader(
		ID3D11Device* device,
		HWND hwnd,
		const WCHAR* vsFilename,
		const WCHAR* psFilename);

public:
	HoloGraphicRenderAPI();
	~HoloGraphicRenderAPI();

	virtual void OnStart() ;

	virtual void OnUpdate();

	virtual void OnDestroy();

	virtual void SetFullScreen();

	virtual void ExitFullScreen() ;

	virtual void OnKeyDown(UINT8 key);

	virtual void OnKeyUp(UINT8 key);

	virtual void OnResize() ;

	


public:
	HWND	m_hwnd;

	// Pipeline objects.	
	// Direct3D 11.1
	ComPtr < ID3D11Device2> m_Device;                  // D3D11.1设备
	ComPtr < ID3D11DeviceContext2> m_DeviceContext; // D3D11.1设备上下文
	ComPtr < IDXGISwapChain1> m_SwapChain;                // D3D11.1交换链

	ComPtr < ID3D11RenderTargetView> m_RenderTargetView;
	ComPtr < ID3D11RenderTargetView> m_RenderTargetViewRight;
	ComPtr < ID3D11DepthStencilView> m_DepthStencilView;


	// Cached renderer properties.
	D3D_FEATURE_LEVEL                m_featureLevel;
	float                                           m_dpi;
	bool                                           m_stereoEnabled;
	static const UINT						  FrameCount = 2;


	D3D11_INPUT_ELEMENT_DESC polygonLayout0[2];
	ComPtr<ID3D11InputLayout>      m_inputLayout0;                //   vertex input layout
	ComPtr<ID3D11Buffer>                m_vertexBuffer0;               //   vertex buffer
	ComPtr<ID3D11Buffer>                m_indexBuffer0;                //   index buffer
	ComPtr<ID3D11VertexShader>    m_vertexShader0;               //   vertex shader
	ComPtr<ID3D11PixelShader>       m_pixelShader0;                //   pixel shader

	ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewLeft0;  // left texture view
	ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewRight0;  // right texture view

	ComPtr<ID3D11SamplerState>          m_sampler0;                    //   texture sampler
	ComPtr<ID3D11Buffer>                      m_constantBuffer0;             // constant buffer resource
	ConstantBuffer   m_constantBufferData0;          // constant buffer resource data
	unsigned int       m_indexCount0;                  //   index count

	D3D11_INPUT_ELEMENT_DESC polygonLayout1[2];
	ComPtr<ID3D11InputLayout>      m_inputLayout1;                //   vertex input layout
	ComPtr<ID3D11Buffer>                m_vertexBuffer1;               //   vertex buffer
	ComPtr<ID3D11Buffer>                m_indexBuffer1;                //   index buffer
	ComPtr<ID3D11VertexShader>    m_vertexShader1;               //   vertex shader
	ComPtr<ID3D11PixelShader>       m_pixelShader1;                //   pixel shader

	ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewLeft1;  // left texture view
	ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewRight1;  // right texture view


	ComPtr<ID3D11SamplerState>          m_sampler1;                    //   texture sampler
	ComPtr<ID3D11Buffer>                      m_constantBuffer1;             // constant buffer resource
	ConstantBuffer   m_constantBufferData1;          // constant buffer resource data
	unsigned int       m_indexCount1;                  //   index count


	float                    m_projAspect;                  // aspect ratio for projection matrix
	float                    m_nearZ;                       // nearest Z-distance at which to draw vertices
	float                    m_farZ;                        // farthest Z-distance at which to draw vertices

	bool					   m_windowSizeChangeInProgress;
	Size                     m_renderTargetSize;
	RECT                   m_windowBounds;
	Size					   m_Resolution;
};