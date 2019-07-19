#include "pch.h"
 

 
#include <math.h>
#include "Sample3DSceneRenderer.h"

#include "BasicShapes.h"
#include "BasicLoader.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Graphics::Display;
using namespace D2D1;
using namespace DirectX;
 

using namespace XinYueEngineUWP;

using namespace DirectX;
using namespace Windows::Foundation;

// 从文件中加载顶点和像素着色器，然后实例化立方体几何图形。
Sample3DSceneRenderer::Sample3DSceneRenderer()
{
	m_stereoExaggerationFactor = 1.0f;

	// Developer decided world unit: in this case, modeled in feet.
	// One world unit equals 1 foot. Therefore, m_worldScale * inches = 1 world unit.
	m_worldScale = 12.0f;
}

// 当窗口的大小改变时初始化视图参数。
void Sample3DSceneRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

}

void XinYueEngineUWP::Sample3DSceneRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	

	BasicLoader^ loader = ref new BasicLoader(m_d3dDevice.Get());

	loader->LoadShader(
		L"SampleVertexShader.cso",
		nullptr,
		0,
		&m_vertexShader,
		&m_inputLayout
	);

	// Create the vertex and index buffers for drawing the cube.
	BasicShapes^ shapes = ref new BasicShapes(m_d3dDevice.Get());

	shapes->CreatePlane(
		&m_vertexBuffer,
		&m_indexBuffer,
		nullptr,
		&m_indexCount
	);

	// Create the constant buffer for updating model and camera data.
	CD3D11_BUFFER_DESC constantBufferDescription(sizeof(ConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
			&constantBufferDescription,
			nullptr, // Leave the buffer uninitialized.
			&m_constantBuffer
		)
	);

	loader->LoadShader(
		L"SamplePixelShader.cso",
		&m_pixelShader
	);

	 

	loader->LoadTexture(
		L"LeftEye.png",
		nullptr,
		&m_textureShaderResourceViewLeft
	);
	loader->LoadTexture(
		L"RightEye.png",
		nullptr,
		&m_textureShaderResourceViewRight
	);

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

	DX::ThrowIfFailed(
		m_d3dDevice->CreateSamplerState(
			&samplerDescription,
			&m_sampler
		)
	);


}

void XinYueEngineUWP::Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	if (m_stereoEnabled)
	{
		m_hintMessage = "Press up/down arrow keys to adjust stereo 3D exaggeration effect";
	}
	else
	{
		m_hintMessage = "Stereo 3D is not enabled on your system";
	}

	m_projAspect = static_cast<float>(m_renderTargetSize.Width) / static_cast<float>(m_renderTargetSize.Height);
	m_nearZ = 0.01f;
	m_farZ = 100.0f;

	// Initialize the view matrix.
	XMFLOAT3 Eye = XMFLOAT3(0.0f, 0.0f, 3.0f);
	XMFLOAT3 At = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(
		&m_constantBufferData.view,
		XMMatrixLookAtRH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up))
	);

	// Set camera parameters.
	m_widthInInches = m_renderTargetSize.Width / m_dpi;
	m_heightInInches = m_renderTargetSize.Height / m_dpi;

	// Create the projection matrix parameters and set up the initial/mono projection matrix.
	StereoParameters params = CreateDefaultStereoParameters(m_widthInInches, m_heightInInches, m_worldScale, 0); // Mono uses zero exaggeration.
	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		StereoProjectionFieldOfViewRightHand(params, m_nearZ, m_farZ, false) // Channel parameter doesn't matter for mono.
	);
	// Rotate the cube.
	XMStoreFloat4x4(
		&m_constantBufferData.model,
		XMMatrixRotationY(0)
	);
	// Transpose the matrices in the constant buffer.
	ConstantBuffer constantBuffer;
	XMStoreFloat4x4(
		&constantBuffer.model,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData.model))
	);
	XMStoreFloat4x4(
		&constantBuffer.view,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData.view))
	);
	XMStoreFloat4x4(
		&constantBuffer.projection,
		XMMatrixTranspose(XMLoadFloat4x4(&m_constantBufferData.projection))
	);

	m_d3dContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &constantBuffer, 0, 0);
}

void XinYueEngineUWP::Sample3DSceneRenderer::Render()
{
	 
}

float XinYueEngineUWP::Sample3DSceneRenderer::GetStereoExaggeration()
{
	return m_stereoExaggerationFactor;
}

void XinYueEngineUWP::Sample3DSceneRenderer::RenderEye(unsigned int eyeIndex)
{
	ComPtr<ID3D11RenderTargetView> currentRenderTargetView;

	// If eyeIndex == 1, set right render target view. Otherwise, set left render target view.
	currentRenderTargetView = eyeIndex ? m_d3dRenderTargetViewRight : m_d3dRenderTargetView;

	// Bind the render targets.
	m_d3dContext->OMSetRenderTargets(
		1,
		currentRenderTargetView.GetAddressOf(),
		m_d3dDepthStencilView.Get()
	);

	// Clear both the render target and depth stencil to default values.
	const float ClearColor[4] = { 0.071f, 0.040f, 0.561f, 1.0f };

	m_d3dContext->ClearRenderTargetView(
		currentRenderTargetView.Get(),
		ClearColor
	);

	m_d3dContext->ClearDepthStencilView(
		m_d3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
	);

	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	// Set the vertex and index buffers.
	UINT stride = sizeof(BasicVertex);
	UINT offset = 0;
	m_d3dContext->IASetVertexBuffers(
		0,                              // Start at the first vertex buffer slot.
		1,                              // Set one vertex buffer binding.
		m_vertexBuffer.GetAddressOf(),
		&stride,                        // Specify the size in bytes of a single vertex.
		&offset                         // Specify the base vertex in the buffer.
	);

	m_d3dContext->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT,   // Specify unsigned short index format.
		0                       // Specify the base index in the buffer.
	);

	// Specify the way the vertex and index buffers define geometry.
	m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the vertex shader stage state.
	m_d3dContext->VSSetShader(
		m_vertexShader.Get(),
		nullptr,                // Don't use shader linkage.
		0                       // Don't use shader linkage.
	);

	m_d3dContext->VSSetConstantBuffers(
		0,                          // Start at the first constant buffer slot.
		1,                          // Set one constant buffer binding.
		m_constantBuffer.GetAddressOf()
	);

	// Set the pixel shader stage state.
	m_d3dContext->PSSetShader(
		m_pixelShader.Get(),
		nullptr,                // Don't use shader linkage.
		0                       // Don't use shader linkage.
	);

	m_d3dContext->PSSetShaderResources(
		0,                          // Start at the first shader resource slot.
		1,                          // Set one shader resource binding.
		eyeIndex==0?m_textureShaderResourceViewLeft.GetAddressOf(): m_textureShaderResourceViewRight.GetAddressOf()
	);

	m_d3dContext->PSSetSamplers(
		0,                          // Starting at the first sampler slot.
		1,                          // Set one sampler binding.
		m_sampler.GetAddressOf()
	);

	// Draw the cube.
	m_d3dContext->DrawIndexed(
		m_indexCount,   // Draw all created vertices.
		0,              // Start with the first vertex.
		0               // Start with the first index.
	);


}

void XinYueEngineUWP::Sample3DSceneRenderer::SetStereoExaggeration(float currentExaggeration)
{
	currentExaggeration = min(currentExaggeration, 4.0f);
	currentExaggeration = max(currentExaggeration, -4.0f);
	m_stereoExaggerationFactor = currentExaggeration;
}

void XinYueEngineUWP::Sample3DSceneRenderer::Update(unsigned int eyeIndex, float timeTotal, float timeDelta)
{

 

}

 
 
 
 