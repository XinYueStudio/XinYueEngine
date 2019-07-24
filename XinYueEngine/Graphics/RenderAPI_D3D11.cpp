#include "RenderAPI.h"
#include "PlatformBase.h"


#if SUPPORT_D3D11
#include "RenderAPI_D3D11.h"


RenderAPI* CreateRenderAPI_D3D11(HWND hwnd, Size resolution, bool stereo)
{
	RenderAPI_D3D11* 	m_D3D11 = new RenderAPI_D3D11();

	m_D3D11->Init(hwnd, resolution, stereo);

	return m_D3D11;
}

ID3D11Device * RenderAPI_D3D11::GetDevice()
{
	return m_Device.Get();
}

void * RenderAPI_D3D11::GetRenderDevice()
{
	if (s_D3D11)
	{
		return s_D3D11->GetDevice();
	}
	return nullptr;
}

RenderAPI_D3D11::RenderAPI_D3D11()
{

}

 

void RenderAPI_D3D11::Init(HWND hwnd, Size screensize, bool stereo)
{
	m_Hwnd = hwnd;
	m_Resolution = screensize;
	m_StereoEnabled = stereo;

	// This flag adds support for surfaces with a different color channel ordering
  // than the API default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	ComPtr<IDXGIDevice> dxgiDevice;

#if defined(_DEBUG)
	if (SdkLayersAvailable())
	{
		// If the project is in a debug build, enable debugging via SDK Layers with this flag.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description.  All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Create the Direct3D 11 API device object and a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ThrowIfFailed(
		D3D11CreateDevice(
			nullptr,                    // Specify nullptr to use the default adapter.
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags,              // Set debug and Direct2D compatibility flags.
			featureLevels,              // List of feature levels this app can support.
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
			&device,                    // Returns the Direct3D device created.
			&m_FeatureLevel,            // Returns feature level of device created.
			&context                    // Returns the device immediate context.
		)
	);
	// Get the Direct3D 11.1 API device and context interfaces.
	ThrowIfFailed(
		device.As(&m_Device)
	);

	ThrowIfFailed(
		context.As(&m_DeviceContext)
	);

	// Get the underlying DXGI device of the Direct3D device.
	ThrowIfFailed(
		m_Device.As(&dxgiDevice)
	);


	Resize();

}

void RenderAPI_D3D11::LoadAssets()
{
	float m_Z = 3.0f;
	m_Camera = new Camera();
	m_Camera->SetPosition(0, 0, m_Z);
	m_Camera->SetRotation(0, 0, 0);

	XMFLOAT3 Eye = XMFLOAT3(0.0f, 0.0f, m_Z);
	XMFLOAT3 At = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	float	m_projAspect = static_cast<float>(m_RenderTargetSize.Width) / static_cast<float>(m_RenderTargetSize.Height);
	float	m_nearZ = 0.01f;
	float m_farZ = 100.0f;
	float	fieldOfView = 60;
	float	AngleY = D3DXToRadian(fieldOfView);
	float	frustumHeight = 2.0f * m_Z * tan(AngleY * 0.5f);
	float	frustumWidth = frustumHeight * m_projAspect;

	
	// Initialize the view matrix.
	XMStoreFloat4x4(
		&m_ConstantBuffer.view,
		XMMatrixLookAtRH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up))
	);
	XMStoreFloat4x4(
		&m_ConstantBuffer.projection,
		XMMatrixPerspectiveFovRH(AngleY, m_projAspect, m_nearZ, m_farZ) // Channel parameter doesn't matter for mono.
	);
	 //Rotate the cube.
	XMStoreFloat4x4(
		&m_ConstantBuffer.model,
		XMMatrixRotationY(0)
	);
	// Transpose the matrices in the constant buffer.
	 
	XMStoreFloat4x4(
		&m_ConstantBuffer.model,
		XMMatrixTranspose(XMLoadFloat4x4(&m_ConstantBuffer.model))
	);
	XMStoreFloat4x4(
		&m_ConstantBuffer.view,
		XMMatrixTranspose(XMLoadFloat4x4(&m_ConstantBuffer.view))
	);
	XMStoreFloat4x4(
		&m_ConstantBuffer.projection,
		XMMatrixTranspose(XMLoadFloat4x4(&m_ConstantBuffer.projection))
	);

 
	m_QuadLeft = new Quad();
	m_QuadRight = new Quad();

	const char* LeftTextureFile = "LeftEye.png";
	const char* RightTextureFile = "RightEye.png";

	m_QuadLeft->InitializeFile(m_Device.Get(), LeftTextureFile, RightTextureFile,frustumWidth*0.5f, frustumHeight, Vector2(-frustumWidth*0.25f, 0), m_ConstantBuffer);
	m_QuadRight->InitializeFile(m_Device.Get(), LeftTextureFile, RightTextureFile, frustumWidth*0.5f, frustumHeight, Vector2(frustumWidth * 0.25f, 0), m_ConstantBuffer);

}

void RenderAPI_D3D11::Resize()
{

	// If the swap chain already exists, resize it.
	if (m_SwapChain != nullptr)
	{
		// Existing swap chain needs to be resized.
		// Make sure that the dependent objects have been released.
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_RenderTargetView = nullptr;
		m_DepthStencilView = nullptr;

		m_RenderTargetViewRight = nullptr;

		HRESULT hr = m_SwapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			// If the device was removed for any reason, a new device and swapchain will need to be created.
			// Reset these member variables to ensure that SetDpi recreates all resources.
			m_SwapChain = nullptr;

			// Make sure the rendering state has been released.
			m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
			m_DepthStencilView = nullptr;
			m_RenderTargetView = nullptr;
			m_RenderTargetViewRight = nullptr;

			m_DeviceContext->Flush();

			// Everything is set up now. Do not continue execution of this method.
			return;
		}
		else
		{
			ThrowIfFailed(hr);
		}
	}
	else    // Otherwise, create a new one.
	{
		// m_swapChain is nullptr either because it has never been created or because it has been
		// invalidated. Make sure that the dependent objects are also released.
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_RenderTargetView = nullptr;
		m_DepthStencilView = nullptr;
		m_RenderTargetViewRight = nullptr;
		m_DeviceContext->Flush();

		// Allocate a descriptor.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		swapChainDesc.Width = m_Resolution.Width;                                     // Use automatic sizing.
		swapChainDesc.Height = m_Resolution.Height;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // This is the most common swap chain format.
		swapChainDesc.Stereo = m_StereoEnabled;
		swapChainDesc.SampleDesc.Count = 1;                          // Don't use multi-sampling.
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;                               // Use double buffering to minimize latency.
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
		swapChainDesc.Flags = 0;

		// Once the desired swap chain description is configured, it must be created on the same adapter as our D3D Device.

		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		ThrowIfFailed(
			m_Device.As(&dxgiDevice)
		);

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		ThrowIfFailed(
			dxgiDevice->GetAdapter(&dxgiAdapter)
		);

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory2> dxgiFactory;
		ThrowIfFailed(
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
		);

		// Obtain the final swap chain for this window from the DXGI factory.
		ThrowIfFailed(
			dxgiFactory->CreateSwapChainForHwnd(
				m_Device.Get(),
				m_Hwnd,
				&swapChainDesc,
				nullptr,    // allow on all displays
				nullptr,
				&m_SwapChain
			)
		);

		// Ensure that DXGI does not queue more than one frame at a time. This both reduces
		// latency and ensures that the application will only render after each VSync, minimizing
		// power consumption.
		ThrowIfFailed(
			dxgiDevice->SetMaximumFrameLatency(1)
		);
	}


	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(
		m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
	);

	// Create a descriptor for the RenderTargetView.
	CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(
		D3D11_RTV_DIMENSION_TEXTURE2DARRAY,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		0,
		0,
		1
	);

	// Create a view interface on the rendertarget to use on bind for mono or left eye view.
	ThrowIfFailed(
		m_Device->CreateRenderTargetView(
			backBuffer.Get(),
			&renderTargetViewDesc,
			&m_RenderTargetView
		)
	);

	// Stereo swapchains have an arrayed resource, so create a second Render Target
	// for the right eye buffer.
	if (m_StereoEnabled)
	{
		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewRightDesc(
			D3D11_RTV_DIMENSION_TEXTURE2DARRAY,
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0,
			1,
			1
		);

		ThrowIfFailed(
			m_Device->CreateRenderTargetView(
				backBuffer.Get(),
				&renderTargetViewRightDesc,
				&m_RenderTargetViewRight
			)
		);
	}

	// Cache the rendertarget dimensions in our helper class for convenient use.
	D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
	backBuffer->GetDesc(&backBufferDesc);
	m_RenderTargetSize.Width = static_cast<float>(backBufferDesc.Width);
	m_RenderTargetSize.Height = static_cast<float>(backBufferDesc.Height);

	// Create a descriptor for the depth/stencil buffer.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		backBufferDesc.Width,
		backBufferDesc.Height,
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);

	// Allocate a 2-D surface as the depth/stencil buffer.
	ComPtr<ID3D11Texture2D> depthStencil;
	ThrowIfFailed(
		m_Device->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			&depthStencil
		)
	);

	// Create a DepthStencil view on this surface to use on bind.
	auto viewDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D);
	ThrowIfFailed(
		m_Device->CreateDepthStencilView(
			depthStencil.Get(),
			&viewDesc,
			&m_DepthStencilView
		)
	);

	// Create a viewport descriptor of the full window size.
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		static_cast<float>(backBufferDesc.Width),
		static_cast<float>(backBufferDesc.Height)
	);

	// Set the current viewport using the descriptor.
	m_DeviceContext->RSSetViewports(1, &viewport);


	LoadAssets();
}

void RenderAPI_D3D11::Update()
{
	m_QuadLeft->Update(m_Device.Get(),m_ConstantBuffer);
	m_QuadRight->Update(m_Device.Get(), m_ConstantBuffer);


}

void RenderAPI_D3D11::Render(int eyeindex)
{
	ComPtr<ID3D11RenderTargetView> currentRenderTargetView;

	// If eyeIndex == 1, set right render target view. Otherwise, set left render target view.
	currentRenderTargetView = eyeindex==0 ? m_RenderTargetView : m_RenderTargetViewRight;
	// Clear both the render target and depth stencil to default values.
	const float ClearColor[4] = {  0,  0, 0, 0 };
	m_DeviceContext->OMSetRenderTargets(
		1,
		currentRenderTargetView.GetAddressOf(),
		m_DepthStencilView.Get());

	m_DeviceContext->ClearRenderTargetView(
		currentRenderTargetView.Get(),
		ClearColor);

	m_DeviceContext->ClearDepthStencilView(
		m_DepthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0);

	 
	m_Camera->Render();

	m_QuadLeft->Render(m_DeviceContext.Get(), eyeindex);
	m_QuadRight->Render(m_DeviceContext.Get(), eyeindex);

	m_DeviceContext->Flush();
}

void RenderAPI_D3D11::Present()
{
	// The application may optionally specify "dirty" or "scroll" rects to improve efficiency
   // in certain scenarios.  In this sample, however, we do not utilize those features.
	DXGI_PRESENT_PARAMETERS parameters = { 0 };
	parameters.DirtyRectsCount = 0;
	parameters.pDirtyRects = nullptr;
	parameters.pScrollRect = nullptr;
	parameters.pScrollOffset = nullptr;

	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_SwapChain->Present1(1, 0, &parameters);

}

void RenderAPI_D3D11::Destroy()
{
	// Existing swap chain needs to be resized.
		// Make sure that the dependent objects have been released.
	m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	m_RenderTargetView = nullptr;
	m_DepthStencilView = nullptr;
	m_RenderTargetViewRight = nullptr;
	// If the device was removed for any reason, a new device and swapchain will need to be created.
		// Reset these member variables to ensure that SetDpi recreates all resources.
	m_SwapChain = nullptr;

	// Make sure the rendering state has been released.
	m_DepthStencilView = nullptr;
	m_DeviceContext->Flush();

	m_DeviceContext->Release();
	m_Device->Release();
}

#endif