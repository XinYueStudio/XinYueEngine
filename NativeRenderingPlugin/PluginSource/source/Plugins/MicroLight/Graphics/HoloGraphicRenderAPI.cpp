#include "HoloGraphicRenderAPI.h"



HoloGraphicRenderAPI::HoloGraphicRenderAPI()
{
}


HoloGraphicRenderAPI::~HoloGraphicRenderAPI()
{
}



void HoloGraphicRenderAPI::UpdateStereoEnabledStatus()
{

	// First, retrieve the underlying DXGI Device from the Direct3D Device.
	ComPtr<IDXGIDevice1> dxgiDevice;
	ThrowIfFailed(
		m_Device.As(&dxgiDevice)
	);

	// Next, get the associated adapter from the DXGI Device.
	ComPtr<IDXGIAdapter> dxgiAdapter;
	ThrowIfFailed(
		dxgiDevice->GetAdapter(&dxgiAdapter)
	);

	// Next, get the parent factory from the DXGI adapter.
	ComPtr<IDXGIFactory2> dxgiFactory;
	ThrowIfFailed(
		dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
	);

	m_stereoEnabled = dxgiFactory->IsWindowedStereoEnabled() ? true : false;



}

void HoloGraphicRenderAPI::UpdateForWindowSizeChange()
{


	bool previousStereoState = m_stereoEnabled;

	UpdateStereoEnabledStatus();

	RECT	Bounds;
	ThrowIfFailed(GetWindowRect(m_hwnd, &Bounds));

	if (previousStereoState != m_stereoEnabled)
	{
		// Swap chain needs to be recreated so release the existing one.
		// The rest of the dependent resources with be released in CreateWindowSizeDependentResources.
		m_SwapChain = nullptr;
		m_windowSizeChangeInProgress = true;
		CreateWindowSizeDependentResources();
	}
	else if ((Bounds.right - Bounds.left) != (m_windowBounds.right - m_windowBounds.left) ||
		(Bounds.right - Bounds.left) != (m_windowBounds.bottom - m_windowBounds.top))
	{
		m_windowBounds = Bounds;
		m_windowSizeChangeInProgress = true;
		CreateWindowSizeDependentResources();
	}
	m_windowBounds = Bounds;
}

void HoloGraphicRenderAPI::HandleDeviceLost()
{
	// Reset these member variables to ensure that SetDpi recreates all resources.
	float dpi = m_dpi;
	m_dpi = -1.0f;

	m_SwapChain = nullptr;

	// Make sure the rendering state has been released.
	m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_DepthStencilView = nullptr;
	m_RenderTargetView = nullptr;
	m_RenderTargetViewRight = nullptr;

	m_DeviceContext->Flush();

	CreateDeviceResources();

	SetDpi(dpi);

}

void HoloGraphicRenderAPI::CreateDeviceResources()
{
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
			&m_featureLevel,            // Returns feature level of device created.
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



}

void HoloGraphicRenderAPI::CreateWindowSizeDependentResources()
{

	// Store the window bounds so the next time we get a SizeChanged event we can
   // avoid rebuilding everything if the size is identical.
	ThrowIfFailed(GetWindowRect(m_hwnd, &m_windowBounds));

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
			HandleDeviceLost();

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
		swapChainDesc.Stereo = m_stereoEnabled;
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
				m_hwnd,
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
	if (m_stereoEnabled)
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
	m_renderTargetSize.Width = static_cast<float>(backBufferDesc.Width);
	m_renderTargetSize.Height = static_cast<float>(backBufferDesc.Height);

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



	//Load shader
	{
 
		wstring UIShader = L"UIShader.hlsl";

		InitializeShader(m_Device.Get(), m_hwnd, UIShader.data(), UIShader.data());


	}


	// Create the constant buffer for updating model and camera data.
	CD3D11_BUFFER_DESC constantBufferDescription(sizeof(ConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	
	ThrowIfFailed(
		m_Device->CreateBuffer(
			&constantBufferDescription,
			nullptr, // Leave the buffer uninitialized.
			&m_constantBuffer0
		)
	);
	ThrowIfFailed(
		m_Device->CreateBuffer(
			&constantBufferDescription,
			nullptr, // Leave the buffer uninitialized.
			&m_constantBuffer1
		)
	);

	m_projAspect = static_cast<float>(m_renderTargetSize.Width) / static_cast<float>(m_renderTargetSize.Height);
	m_nearZ = 0.01f;
	m_farZ = 1000.0f;

	float aspectRatio = (float)m_renderTargetSize.Width / (float)m_renderTargetSize.Height;
	float	fovInDegrees = 60;
	float fovAngleY = fovInDegrees * XM_PI / 180.0f;
	float Zfar = 1;
	float	frustumHeight = 2.0f * Zfar * tan(fovAngleY * 0.5f);
	float	frustumWidth = frustumHeight * aspectRatio;

	CreatePlane(
		&m_vertexBuffer0,
		&m_indexBuffer0,
		nullptr,
		&m_indexCount0,
		frustumWidth,
		frustumHeight);
	CreatePlane(
		&m_vertexBuffer1,
		&m_indexBuffer1,
		nullptr,
		&m_indexCount1,
		frustumWidth,
		frustumHeight);

	/*ThrowIfFailed(D3DX11CreateShaderResourceViewFromFile(
		m_Device.Get(),
		TEXT("LeftEye.png"),
		NULL,
		NULL,
		&m_textureShaderResourceViewLeft0,
		NULL));

	ThrowIfFailed(D3DX11CreateShaderResourceViewFromFile(
		m_Device.Get(),
		TEXT("RightEye.png"),
		NULL,
		NULL,
		&m_textureShaderResourceViewRight0,
		NULL));

	ThrowIfFailed(D3DX11CreateShaderResourceViewFromFile(
		m_Device.Get(),
		TEXT("LeftEye.png"),
		NULL,
		NULL,
		&m_textureShaderResourceViewLeft1,
		NULL));

	ThrowIfFailed(D3DX11CreateShaderResourceViewFromFile(
		m_Device.Get(),
		TEXT("RightEye.png"),
		NULL,
		NULL,
		&m_textureShaderResourceViewRight1,
		NULL));
*/

	// Create the sampler.
	D3D11_SAMPLER_DESC samplerDescription;
	ZeroMemory(&samplerDescription, sizeof(samplerDescription));
	samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MaxAnisotropy = m_featureLevel > D3D_FEATURE_LEVEL_9_1 ? 4 : 2;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescription.BorderColor[0] = 0.0f;
	samplerDescription.BorderColor[1] = 0.0f;
	samplerDescription.BorderColor[2] = 0.0f;
	samplerDescription.BorderColor[3] = 0.0f;
	samplerDescription.MinLOD = 0; // Allow use of all MIP levels.
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	ThrowIfFailed(
		m_Device->CreateSamplerState(
			&samplerDescription,
			&m_sampler0
		)
	);

	ThrowIfFailed(
		m_Device->CreateSamplerState(
			&samplerDescription,
			&m_sampler1
		)
	);



	// Initialize the view matrix.
	XMFLOAT3 Eye = XMFLOAT3(0.0f, 0.0f, Zfar);
	XMFLOAT3 At = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(
		&m_constantBufferData0.view,
		XMMatrixLookAtRH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up))
	);
	XMStoreFloat4x4(
		&m_constantBufferData1.view,
		XMMatrixLookAtRH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up))
	);


	XMStoreFloat4x4(
		&m_constantBufferData0.projection,
		XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, m_nearZ, m_farZ) // Channel parameter doesn't matter for mono.
	);
	XMStoreFloat4x4(
		&m_constantBufferData1.projection,
		XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, m_nearZ, m_farZ) // Channel parameter doesn't matter for mono.
	);
	// Rotate model Matrix.
	XMStoreFloat4x4(
		&m_constantBufferData0.model,
		XMMatrixRotationY(0)
	);
	// Rotate model Matrix.
	XMStoreFloat4x4(
		&m_constantBufferData1.model,
		XMMatrixRotationY(0)
	);
	// Transpose the matrices in the constant buffer.
	ConstantBuffer constantBuffer;
	XMStoreFloat4x4(
		&constantBuffer.model,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData0.model))
	);
	XMStoreFloat4x4(
		&constantBuffer.view,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData0.view))
	);
	XMStoreFloat4x4(
		&constantBuffer.projection,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData0.projection))
	);

	m_DeviceContext->UpdateSubresource(m_constantBuffer0.Get(), 0, nullptr, &constantBuffer, 0, 0);


	XMStoreFloat4x4(
		&constantBuffer.model,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData1.model))
	);
	XMStoreFloat4x4(
		&constantBuffer.view,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData1.view))
	);
	XMStoreFloat4x4(
		&constantBuffer.projection,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData1.projection))
	);

	m_DeviceContext->UpdateSubresource(m_constantBuffer1.Get(), 0, nullptr, &constantBuffer, 0, 0);

}

void HoloGraphicRenderAPI::SetDpi(float dpi)
{

	// Save the DPI of this display in our class.


	m_dpi = dpi;


	// Often a DPI change implies a window size change. In some cases Windows will issue
	// both a size changed event and a DPI changed event. In this case, the resulting bounds
	// will not change, and the window resize code will only be executed once.
	UpdateForWindowSizeChange();


}

 

void HoloGraphicRenderAPI::LoadPipeline()
{

	CreateDeviceResources();

	float dpi = GetDpiForWindow(m_hwnd);

	SetDpi(dpi);

}

void HoloGraphicRenderAPI::OnResize()
{

	float dpi = GetDpiForWindow(m_hwnd);

	SetDpi(dpi);

}

void HoloGraphicRenderAPI::OnKeyDown(UINT8 key)
{


}
void HoloGraphicRenderAPI::OnKeyUp(UINT8 key)
{


}


void HoloGraphicRenderAPI::CreateVertexBuffer(
	_In_ unsigned int numVertices,
	_In_ BasicVertex *vertexData,
	_Out_ ID3D11Buffer **vertexBuffer
)
{
	*vertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBufferInternal;

	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.ByteWidth = sizeof(BasicVertex) * numVertices;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexBufferData;
	VertexBufferData.pSysMem = vertexData;
	VertexBufferData.SysMemPitch = 0;
	VertexBufferData.SysMemSlicePitch = 0;

	ThrowIfFailed(
		m_Device->CreateBuffer(
			&VertexBufferDesc,
			&VertexBufferData,
			&vertexBufferInternal
		)
	);


	*vertexBuffer = vertexBufferInternal.Detach();
}

void HoloGraphicRenderAPI::CreateIndexBuffer(
	_In_ unsigned int numIndices,
	_In_ unsigned short *indexData,
	_Out_ ID3D11Buffer **indexBuffer
)
{
	*indexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBufferInternal;

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.ByteWidth = sizeof(unsigned short) * numIndices;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA IndexBufferData;
	IndexBufferData.pSysMem = indexData;
	IndexBufferData.SysMemPitch = 0;
	IndexBufferData.SysMemSlicePitch = 0;

	ThrowIfFailed(
		m_Device->CreateBuffer(
			&IndexBufferDesc,
			&IndexBufferData,
			&indexBufferInternal
		)
	);

	*indexBuffer = indexBufferInternal.Detach();
}


void HoloGraphicRenderAPI::CreatePlane(ID3D11Buffer ** vertexBuffer, ID3D11Buffer ** indexBuffer, unsigned int * vertexCount, unsigned int * indexCount, float	frustumWidth, float	frustumHeight)
{


	BasicVertex axisVertices[] =
	{
		{ Vector3(-frustumWidth * 0.5f,  frustumHeight *0.5f, 0), Vector2(0.0f, 0.0f) },
		{ Vector3(frustumWidth *0.5f, frustumHeight * 0.5f, 0), Vector2(1.0f, 0.0f) },
		{ Vector3(frustumWidth *0.5f, -frustumHeight * 0.5f, 0), Vector2(1.0f, 1.0f) },
		{ Vector3(-frustumWidth * 0.5f, -frustumHeight * 0.5f, 0), Vector2(0.0f, 1.0f) },
	};

	unsigned short axisIndices[] =
	{
		   0, 1, 2,
		0, 2, 3,
	};

	CreateVertexBuffer(
		ARRAYSIZE(axisVertices),
		axisVertices,
		vertexBuffer
	);
	if (vertexCount != nullptr)
	{
		*vertexCount = ARRAYSIZE(axisVertices);
	}

	CreateIndexBuffer(
		ARRAYSIZE(axisIndices),
		axisIndices,
		indexBuffer
	);
	if (indexCount != nullptr)
	{
		*indexCount = ARRAYSIZE(axisIndices);
	}
}

bool HoloGraphicRenderAPI::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "vert", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{

		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "frag", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{

		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader0);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader0);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader1);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader1);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_inputLayout0);

 
	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_inputLayout1);

	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;




	return true;
}

void HoloGraphicRenderAPI::LoadAssets()
{


}

void HoloGraphicRenderAPI::PopulateCommandList()
{



}

void HoloGraphicRenderAPI::WaitForPreviousFrame()
{


}
 
 
/**********************************************************************************/
/**********************************RenderAPI*************************************/
/**********************************************************************************/
void HoloGraphicRenderAPI::OnStart()
{

	LoadPipeline();
	LoadAssets();

}


void HoloGraphicRenderAPI::RenderEye(int eyeIndex)
{
	ComPtr<ID3D11RenderTargetView> currentRenderTargetView;

	// If eyeIndex == 1, set right render target view. Otherwise, set left render target view.
	currentRenderTargetView = eyeIndex ? m_RenderTargetView : m_RenderTargetViewRight;
	// Clear both the render target and depth stencil to default values.
	const float ClearColor[4] = { 0.071f, 0.040f, 0.561f, 1.0f };
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


	m_DeviceContext->IASetInputLayout(m_inputLayout.Get());


	// Set the vertex and index buffers.
	UINT stride = sizeof(BasicVertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(
		0,                              // Start at the first vertex buffer slot.
		1,                              // Set one vertex buffer binding.
		m_vertexBuffer.GetAddressOf(),
		&stride,                        // Specify the size in bytes of a single vertex.
		&offset                         // Specify the base vertex in the buffer.
	);

	m_DeviceContext->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT,   // Specify unsigned short index format.
		0                       // Specify the base index in the buffer.
	);

	// Specify the way the vertex and index buffers define geometry.
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the vertex shader stage state.
	m_DeviceContext->VSSetShader(
		m_vertexShader.Get(),
		nullptr,                // Don't use shader linkage.
		0                       // Don't use shader linkage.
	);

	m_DeviceContext->VSSetConstantBuffers(
		0,                          // Start at the first constant buffer slot.
		1,                          // Set one constant buffer binding.
		m_constantBuffer.GetAddressOf()
	);

	// Set the pixel shader stage state.
	m_DeviceContext->PSSetShader(
		m_pixelShader.Get(),
		nullptr,                // Don't use shader linkage.
		0                       // Don't use shader linkage.
	);

	m_DeviceContext->PSSetShaderResources(
		0,                          // Start at the first shader resource slot.
		1,                          // Set one shader resource binding.
		eyeIndex == 0 ? m_textureShaderResourceViewLeft.GetAddressOf() : m_textureShaderResourceViewRight.GetAddressOf()
	);

	m_DeviceContext->PSSetSamplers(
		0,                          // Starting at the first sampler slot.
		1,                          // Set one sampler binding.
		m_sampler.GetAddressOf()
	);

	// Draw the cube.
	m_DeviceContext->DrawIndexed(
		m_indexCount,   // Draw all created vertices.
		0,              // Start with the first vertex.
		0               // Start with the first index.
	);


	// Update the system stats.


	m_DeviceContext->Flush();

}
void HoloGraphicRenderAPI::OnUpdate()
{

	RenderEye(0);

	if (m_stereoEnabled)
	{
		RenderEye(1);
	}
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

	WaitForPreviousFrame();


}

void HoloGraphicRenderAPI::OnDestroy()
{

	// Ensure that the GPU is no longer referencing resources that are about to be
	// cleaned up by the destructor.
	WaitForPreviousFrame();




}

void HoloGraphicRenderAPI::SetFullScreen()
{
	if (m_SwapChain == NULL)return;
	// Detect if newly created full-screen swap chain isn't actually full screen.
	IDXGIOutput* pTarget; BOOL bFullscreen;
	if (SUCCEEDED(m_SwapChain->GetFullscreenState(&bFullscreen, &pTarget)))
	{
		if (pTarget)
			pTarget->Release();
	}
	else
		bFullscreen = FALSE;
	// If not full screen, enable full screen again.
	if (!bFullscreen)
	{

		SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0L);

		m_SwapChain->SetFullscreenState(TRUE, NULL);

	}
}

void HoloGraphicRenderAPI::ExitFullScreen()
{
	if (m_SwapChain == NULL)return;
	// Detect if newly created full-screen swap chain isn't actually full screen.
	IDXGIOutput* pTarget; BOOL bFullscreen;
	if (SUCCEEDED(m_SwapChain->GetFullscreenState(&bFullscreen, &pTarget)))
	{
		pTarget->Release();
	}
	else
		bFullscreen = FALSE;
	// If not full screen, enable full screen again.
	if (bFullscreen)
	{
		ShowWindow(m_hwnd, SW_MINIMIZE);
		ShowWindow(m_hwnd, SW_MAXIMIZE);
		ShowWindow(m_hwnd, SW_RESTORE);
		m_SwapChain->SetFullscreenState(false, NULL);
	}
}




