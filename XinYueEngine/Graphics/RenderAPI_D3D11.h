#pragma once
#include "RenderAPI.h"
#include "PlatformBase.h"

// Direct3D 11 implementation of RenderAPI.

#if SUPPORT_D3D11

//Include and link appropriate libraries and headers//
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "D3dx9.lib")
#pragma comment(lib, "Dxgi.lib")
#include <assert.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <d3dx11.h>
#include <d3d11_2.h>
 
#include <D3DX10.h>
#include <D3dx9math.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;
 

#include <vector>
#include <assert.h>

#include "IXinYueGraphicsD3D11.h"
#include "CompileD3DShader.h"

#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "textclass.h"

const BYTE KVSCODE[] =
{
	68,88,66,67,-73,-13,-33,53,50,-44,-122,-105,-77,53,75,-21,-10,48,-108,118,1,0,0,0,32,5,0,0,5,0,0,0,52,0,0,0,
	-72,1,0,0,12,2,0,0,100,2,0,0,-124,4,0,0,82,68,69,70,124,1,0,0,1,0,0,0,108,0,0,0,1,0,0,0,60,0,0,0,0,5,-2,-1,0,9,0,
	0,72,1,0,0,82,68,49,49,60,0,0,0,24,0,0,0,32,0,0,0,40,0,0,0,36,0,0,0,12,0,0,0,0,0,0,0,92,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,77,97,116,114,105,120,66,117,102,102,101,114,0,-85,-85,-85,92,0,0,0,3,0,
	0,0,-124,0,0,0,-64,0,0,0,0,0,0,0,0,0,0,0,-4,0,0,0,0,0,0,0,64,0,0,0,2,0,0,0,8,1,0,0,0,0,0,0,-1,-1,-1,-1,0,0,0,0,-1,-1,
	-1,-1,0,0,0,0,44,1,0,0,64,0,0,0,64,0,0,0,2,0,0,0,8,1,0,0,0,0,0,0,-1,-1,-1,-1,0,0,0,0,-1,-1,-1,-1,0,0,0,0,55,1,0,0,
	-128,0,0,0,64,0,0,0,2,0,0,0,8,1,0,0,0,0,0,0,-1,-1,-1,-1,0,0,0,0,-1,-1,-1,-1,0,0,0,0,119,111,114,108,100,77,97,
	116,114,105,120,0,3,0,3,0,4,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,118,105,101,119,77,
	97,116,114,105,120,0,112,114,111,106,101,99,116,105,111,110,77,97,116,114,105,120,0,77,105,99,114,
	111,115,111,102,116,32,40,82,41,32,72,76,83,76,32,83,104,97,100,101,114,32,67,111,109,112,105,108,
	101,114,32,57,46,50,57,46,57,53,50,46,51,49,49,49,0,-85,-85,-85,73,83,71,78,76,0,0,0,2,0,0,0,8,0,0,0,56,
	0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,15,7,0,0,65,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,1,0,0,0,3,3,0,0,80,79,83,73,84,73,
	79,78,0,84,69,88,67,79,79,82,68,0,-85,-85,79,83,71,78,80,0,0,0,2,0,0,0,8,0,0,0,56,0,0,0,0,0,0,0,1,0,0,0,3,0,0,
	0,0,0,0,0,15,0,0,0,68,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,1,0,0,0,3,12,0,0,83,86,95,80,79,83,73,84,73,79,78,0,84,69,
	88,67,79,79,82,68,0,-85,-85,-85,83,72,69,88,24,2,0,0,80,0,1,0,-122,0,0,0,106,8,0,1,89,0,0,4,70,-114,32,0,0,
	0,0,0,12,0,0,0,95,0,0,3,114,16,16,0,0,0,0,0,95,0,0,3,50,16,16,0,1,0,0,0,103,0,0,4,-14,32,16,0,0,0,0,0,1,0,0,0,
	101,0,0,3,50,32,16,0,1,0,0,0,104,0,0,2,2,0,0,0,54,0,0,5,114,0,16,0,0,0,0,0,70,18,16,0,0,0,0,0,54,0,0,5,-126,0,
	16,0,0,0,0,0,1,64,0,0,0,0,-128,63,17,0,0,8,18,0,16,0,1,0,0,0,70,14,16,0,0,0,0,0,70,-114,32,0,0,0,0,0,0,0,0,0,
	17,0,0,8,34,0,16,0,1,0,0,0,70,14,16,0,0,0,0,0,70,-114,32,0,0,0,0,0,1,0,0,0,17,0,0,8,66,0,16,0,1,0,0,0,70,14,
	16,0,0,0,0,0,70,-114,32,0,0,0,0,0,2,0,0,0,17,0,0,8,-126,0,16,0,1,0,0,0,70,14,16,0,0,0,0,0,70,-114,32,0,0,0,0,
	0,3,0,0,0,17,0,0,8,18,0,16,0,0,0,0,0,70,14,16,0,1,0,0,0,70,-114,32,0,0,0,0,0,4,0,0,0,17,0,0,8,34,0,16,0,0,0,0,
	0,70,14,16,0,1,0,0,0,70,-114,32,0,0,0,0,0,5,0,0,0,17,0,0,8,66,0,16,0,0,0,0,0,70,14,16,0,1,0,0,0,70,-114,32,0,
	0,0,0,0,6,0,0,0,17,0,0,8,-126,0,16,0,0,0,0,0,70,14,16,0,1,0,0,0,70,-114,32,0,0,0,0,0,7,0,0,0,17,0,0,8,18,32,
	16,0,0,0,0,0,70,14,16,0,0,0,0,0,70,-114,32,0,0,0,0,0,8,0,0,0,17,0,0,8,34,32,16,0,0,0,0,0,70,14,16,0,0,0,0,0,70,
	-114,32,0,0,0,0,0,9,0,0,0,17,0,0,8,66,32,16,0,0,0,0,0,70,14,16,0,0,0,0,0,70,-114,32,0,0,0,0,0,10,0,0,0,17,0,0,8,
	-126,32,16,0,0,0,0,0,70,14,16,0,0,0,0,0,70,-114,32,0,0,0,0,0,11,0,0,0,54,0,0,5,50,32,16,0,1,0,0,0,70,16,16,0,
	1,0,0,0,62,0,0,1,83,84,65,84,-108,0,0,0,16,0,0,0,2,0,0,0,0,0,0,0,4,0,0,0,12,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

const BYTE KPSCODE[] =
{
  68,88,66,67,47,115,109,-121,-115,18,37,63,65,-84,-53,100,-39,-86,-103,-30,1,0,0,0,-92,2,0,0,5,0,0,0,52,0,0,0,
  4,1,0,0,92,1,0,0,-112,1,0,0,8,2,0,0,82,68,69,70,-56,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,60,0,0,0,0,5,-1,-1,0,9,0,0,-107,0,
  0,0,82,68,49,49,60,0,0,0,24,0,0,0,32,0,0,0,40,0,0,0,36,0,0,0,12,0,0,0,0,0,0,0,124,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,0,0,0,0,0,0,0,-121,0,0,0,2,0,0,0,5,0,0,0,4,0,0,0,-1,-1,-1,-1,0,0,0,0,1,0,0,0,12,0,0,0,83,97,109,112,108,
  101,84,121,112,101,0,115,104,97,100,101,114,84,101,120,116,117,114,101,0,77,105,99,114,111,115,111,102,
  116,32,40,82,41,32,72,76,83,76,32,83,104,97,100,101,114,32,67,111,109,112,105,108,101,114,32,57,46,50,57,
  46,57,53,50,46,51,49,49,49,0,-85,-85,73,83,71,78,80,0,0,0,2,0,0,0,8,0,0,0,56,0,0,0,0,0,0,0,1,0,0,0,3,0,0,0,0,0,0,0,
  15,0,0,0,68,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,1,0,0,0,3,3,0,0,83,86,95,80,79,83,73,84,73,79,78,0,84,69,88,67,79,79,82,
  68,0,-85,-85,-85,79,83,71,78,44,0,0,0,1,0,0,0,8,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,15,0,0,0,83,86,95,84,
  65,82,71,69,84,0,-85,-85,83,72,69,88,112,0,0,0,80,0,0,0,28,0,0,0,106,8,0,1,90,0,0,3,0,96,16,0,0,0,0,0,88,24,0,4,0,
  112,16,0,0,0,0,0,85,85,0,0,98,16,0,3,50,16,16,0,1,0,0,0,101,0,0,3,-14,32,16,0,0,0,0,0,69,0,0,-117,-62,0,0,-128,67,
  85,21,0,-14,32,16,0,0,0,0,0,70,16,16,0,1,0,0,0,70,126,16,0,0,0,0,0,0,96,16,0,0,0,0,0,62,0,0,1,83,84,65,84,-108,0,0,
  0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


class RenderAPI_D3D11 : public RenderAPI, public IXinYueGraphicsD3D11
{

private:


	virtual	void UpdateStereoEnabledStatus();

	virtual	void UpdateForWindowSizeChange();

	virtual void HandleDeviceLost();

	virtual void CreateDeviceResources();

	virtual void CreateWindowSizeDependentResources();

	virtual void SetDpi(float dpi);

	virtual void SetStereoExaggeration(float Factor);

	virtual void LoadPipeline();

	virtual void LoadAssets();

	virtual void PopulateCommandList();

	virtual 	void WaitForPreviousFrame();

public:

	virtual ~RenderAPI_D3D11() { }

	virtual void SetHWND(HWND hwnd);

	/**********************************************************************************/
	/***************************IXinYueGraphicsD3D11******************************/
	/**********************************************************************************/
	virtual void* GetRenderDevice();

	virtual	ID3D11Device*  GetDevice();

	/**********************************************************************************/
	/**********************************RenderAPI*************************************/
	/**********************************************************************************/
	virtual void OnStart();
 
	virtual void RenderEye(int eyeindex);

	virtual void OnUpdate();

	virtual void OnDestroy();

	virtual void SetFullScreen();

	virtual void ExitFullScreen();

	virtual void OnResize();

	virtual void OnKeyDown(UINT8 key);

	virtual void OnKeyUp(UINT8 key);
	

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

private:
	IXinYueGraphicsD3D11* s_D3D11;
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


	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	ComPtr<ID3D11InputLayout>      m_inputLayout;                //   vertex input layout
	ComPtr<ID3D11Buffer>                m_vertexBuffer;               //   vertex buffer
	ComPtr<ID3D11Buffer>                m_indexBuffer;                //   index buffer
	ComPtr<ID3D11VertexShader>    m_vertexShader;               //   vertex shader
	ComPtr<ID3D11PixelShader>       m_pixelShader;                //   pixel shader

	ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewLeft;  // left texture view
	ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewRight;  // right texture view

	ComPtr<ID3D11SamplerState>          m_sampler;                    //   texture sampler
	ComPtr<ID3D11Buffer>                      m_constantBuffer;             // constant buffer resource

	unsigned int       m_indexCount;                  //   index count
	ConstantBuffer   m_constantBufferData;          // constant buffer resource data
	float                    m_projAspect;                  // aspect ratio for projection matrix
	float                    m_nearZ;                       // nearest Z-distance at which to draw vertices
	float                    m_farZ;                        // farthest Z-distance at which to draw vertices
	float                    m_widthInInches;               // estimated screen width in inches
	float                    m_heightInInches;              // estimated screen height in inches
	float                    m_stereoExaggerationFactor;    // stereo effect that is user adjustable
	 
	bool					   m_windowSizeChangeInProgress;
    Size                     m_renderTargetSize;
	RECT                   m_windowBounds;
 
 
};

#endif