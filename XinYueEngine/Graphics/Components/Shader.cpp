#include "Shader.h"
#include <d3dx10math.h>
#include <d3dx11async.h>

Shader::Shader()
{
}
bool Shader:: Initialize(ID3D11Device2* device, const WCHAR* shaderReources, ConstantBuffer  constantBuffer)
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
	result = D3DX11CompileFromFile(shaderReources, NULL, NULL, "vert", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{

		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(NULL, shaderReources, L"Missing Shader File", MB_OK);
		}

		return false ;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(shaderReources, NULL, NULL, "frag", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{

		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(NULL, shaderReources, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return  false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return  false;
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
		&m_inputLayout);

	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	//HRESULT hr = S_OK;
	////// shaders	 
	//hr = device->CreateVertexShader(
	//	KVSCODE,
	//	sizeof(KVSCODE),
	//	nullptr,
	//	&m_vertexShader);

	//if (FAILED(hr))
	//	OutputDebugStringA("Failed to create vertex shader.\n");

	//hr = device->CreatePixelShader(
	//	KPSCODE,
	//	sizeof(KPSCODE),
	//	nullptr,
	//	&m_pixelShader);
	//if (FAILED(hr))
	//	OutputDebugStringA("Failed to create pixel shader.\n");
	//// Create the vertex input layout.

	//if (m_vertexShader)
	//{
	//	// Create the vertex input layout description.
	//   // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	//	polygonLayout[0].SemanticName = "POSITION";
	//	polygonLayout[0].SemanticIndex = 0;
	//	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//	polygonLayout[0].InputSlot = 0;
	//	polygonLayout[0].AlignedByteOffset = 0;
	//	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//	polygonLayout[0].InstanceDataStepRate = 0;

	//	polygonLayout[1].SemanticName = "TEXCOORD";
	//	polygonLayout[1].SemanticIndex = 0;
	//	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	//	polygonLayout[1].InputSlot = 0;
	//	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	//	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//	polygonLayout[1].InstanceDataStepRate = 0;

	//	// Get a count of the elements in the layout.
	//	int	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//	// Create the vertex input layout.
	//	hr = device->CreateInputLayout(polygonLayout, numElements, KVSCODE, sizeof(KVSCODE),
	//		&m_inputLayout);

	//}



	// Create the constant buffer for updating model and camera data.
	CD3D11_BUFFER_DESC constantBufferDescription(sizeof(ConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	ThrowIfFailed(
		device->CreateBuffer(
			&constantBufferDescription,
			nullptr, // Leave the buffer uninitialized.
			&m_constantBuffer
		)
	);

	// Create the sampler.
	D3D11_SAMPLER_DESC samplerDescription;
	ZeroMemory(&samplerDescription, sizeof(samplerDescription));
	samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MaxAnisotropy = 2;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescription.BorderColor[0] = 0.0f;
	samplerDescription.BorderColor[1] = 0.0f;
	samplerDescription.BorderColor[2] = 0.0f;
	samplerDescription.BorderColor[3] = 0.0f;
	samplerDescription.MinLOD = 0; // Allow use of all MIP levels.
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	ThrowIfFailed(
		device->CreateSamplerState(
			&samplerDescription,
			&m_sampler
		)
	);


	 

	ID3D11DeviceContext2* deviceContext;
	device->GetImmediateContext2(&deviceContext);
	deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &constantBuffer, 0, 0);

 
 
	return true;
}



Shader::~Shader()
{
}

void Shader::Update(ID3D11Device2* device,ConstantBuffer  constantBuffer)
{
	ID3D11DeviceContext2* deviceContext;
	device->GetImmediateContext2(&deviceContext);
	deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &constantBuffer, 0, 0);



	
}

 
void Shader::Render(ID3D11DeviceContext2* deviceContext, int eyeindex)
{
 
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_inputLayout.Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
 
	deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, eyeindex==0?&LeftRT->m_Texture: &RightRT->m_Texture);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, m_sampler.GetAddressOf());

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
 
}

 