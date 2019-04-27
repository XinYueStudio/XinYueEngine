#include "RenderAPI.h"
#include "PlatformBase.h"

// Direct3D 11 implementation of RenderAPI.

#if SUPPORT_D3D11


#include <assert.h>
#include <d3d11.h>
//Include and link appropriate libraries and headers//
#pragma comment(lib, "d3d11.lib")
 


#include "IXinYueGraphicsD3D11.h"

class RenderAPI_D3D11 : public RenderAPI, public IXinYueGraphicsD3D11
{
public:

	virtual ~RenderAPI_D3D11() { }
	void SetHWND(HWND hwnd);

	/**********************************************************************************/
	/**********************************RenderAPI*************************************/
	/**********************************************************************************/
	virtual void* GetRenderDevice();

	virtual void OnStart();

	virtual void OnUpdate();

	virtual void OnDestroy();

	/**********************************************************************************/
	/***************************IXinYueGraphicsD3D11******************************/
	/**********************************************************************************/
	virtual	ID3D11Device*  GetDevice();
 
private:
	IXinYueGraphicsD3D11* s_D3D11;

	// Pipeline objects.
	IDXGISwapChain* m_swapChain;
	ID3D11Device*m_device;
	ID3D11RenderTargetView* m_renderTarget;
	ID3D11DeviceContext* m_deviceContext;

	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11CommandList* m_commandList;
	UINT m_rtvDescriptorSize;
	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	UINT64 m_fenceValue;
	
	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();
	void WaitForPreviousFrame();



	HWND	m_hwnd;
};
RenderAPI* CreateRenderAPI_D3D11(HWND hwnd)
{
	RenderAPI_D3D11* 	m_D3D11 = new RenderAPI_D3D11();
	m_D3D11->SetHWND(hwnd);
	return m_D3D11;
}


/**********************************************************************************/
/**********************************RenderAPI*************************************/
/**********************************************************************************/
const UINT kNodeMask = 0;



void RenderAPI_D3D11::SetHWND(HWND hwnd)
{
	m_hwnd = hwnd;

}
void * RenderAPI_D3D11::GetRenderDevice()
{
	if (s_D3D11)
	{
		return s_D3D11->GetDevice();
	}
	return nullptr;
}

void RenderAPI_D3D11::OnStart()
{
	LoadPipeline();
	LoadAssets();
}

void RenderAPI_D3D11::OnUpdate()
{
	// Record all the commands we need to render the scene into the command list.
	//PopulateCommandList();

	// Execute the command list.	 
	//m_deviceContext->ExecuteCommandList(m_commandList.Get(),true);
	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };

	m_deviceContext->ClearRenderTargetView(m_renderTarget, clearColor);
	//
   // Clear the depth buffer to 1.0 (max depth)
   //
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	// Present the frame.

	m_swapChain->Present(0, 0);

	//WaitForPreviousFrame();
}

void RenderAPI_D3D11::OnDestroy()
{

	// Ensure that the GPU is no longer referencing resources that are about to be
	// cleaned up by the destructor.
	WaitForPreviousFrame();

	CloseHandle(m_fenceEvent);
}



/**********************************************************************************/
/***************************IXinYueGraphicsD3D11******************************/
/**********************************************************************************/
ID3D11Device * RenderAPI_D3D11::GetDevice()
{
	return m_device;
}




/**********************************************************************************/
/********************************Private Function*********************************/
/**********************************************************************************/
 

void RenderAPI_D3D11::LoadPipeline()
{
	HRESULT hr = S_OK;
	
	
	UINT dxgiFactoryFlags = 0;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	// Enable additional debug layers.
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	
	// Define the ordering of feature levels that Direct3D attempts to create.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
 

 
 
	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.OutputWindow = m_hwnd;     //�����������
	swapChainDesc.BufferCount = 1;//��������
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//������̨�����ʽ
	swapChainDesc.BufferDesc.Width = m_Resolution.w; //������̨����ֱ��ʿ��
	swapChainDesc.BufferDesc.Height = m_Resolution.h;//������̨����ֱ��ʸ߶�
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;//ˢ��Ƶ�ʵķ�ĸ
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;//ˢ��Ƶ�ʵķ��ӣ����������ÿ��ˢ��60��
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;    //���ﲻʹ�ö��ز��������������Ϊ1
	swapChainDesc.SampleDesc.Quality = 0;  //��ʹ�ö��ز�������Ϊ0
	swapChainDesc.Windowed = true;         //����Ϊ����ģʽ
	 
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, creationFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, &g_featureLevel, &m_deviceContext);
		if (SUCCEEDED(hr))
			break;
	}


	ThrowIfFailed(hr);
	 
	m_frameIndex =0;

	
	// Create a RTV for each frame.
	ID3D11Texture2D* pBackBuffer=NULL;
	ThrowIfFailed(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(&pBackBuffer)));
	hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTarget);
	pBackBuffer->Release();

	ThrowIfFailed(hr);
 

 

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_Resolution.w; //������̨����ֱ��ʿ��
	descDepth.Height = m_Resolution.h;//������̨����ֱ��ʸ߶�
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ThrowIfFailed(m_device->CreateTexture2D(&descDepth, NULL, &m_depthStencilBuffer));
	 
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_DESC  depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// �������ģ��״̬����. 
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//D3D11_DEPTH_WRITE_MASK_ZERO��ֹд��Ȼ��� 
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// ����front face ����ʹ�õ�ģ���������. 
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ����back face����ʹ�õ�ģ�����ģʽ. 
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �������ģ��״̬��ʹ����Ч 
	ThrowIfFailed(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState));

	// �������ģ��״̬. 
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


	D3D11_DEPTH_STENCIL_VIEW_DESC  depthStencilViewDesc;
	// ��ʼ�����ģ����ͼ. 
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// �������ģ����ͼ����. 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// �������ģ����ͼ. 
	ThrowIfFailed(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView));

	m_deviceContext->OMSetRenderTargets(1, &m_renderTarget, m_depthStencilView);
 
	// ���ù�դ��������ָ���������α���Ⱦ. 
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_SOLID 
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// ������դ��״̬ 
	ThrowIfFailed(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState));

	//���ù�դ��״̬��ʹ����Ч 
	m_deviceContext->RSSetState(m_rasterState);


	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_Resolution.w;
	vp.Height = (FLOAT)m_Resolution.h;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &vp);



}
void RenderAPI_D3D11::LoadAssets()
{
	// Create the command list.

 
	// Command lists are created in the recording state, but there is nothing
	// to record yet. The main loop expects it to be closed, so close it now.
	if (m_deviceContext)
	{
		//m_deviceContext->ExecuteCommandList(m_commandList.Get(), TRUE);
	}


}

void RenderAPI_D3D11::PopulateCommandList()
{
 
	
	 
}

void RenderAPI_D3D11::WaitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
   // This is code implemented as such for simplicity. The D3D11HelloFrameBuffering
   // sample illustrates how to use fences for efficient resource usage and to
   // maximize GPU utilization.

 
}

#endif