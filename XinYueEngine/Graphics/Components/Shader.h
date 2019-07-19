#pragma once
#include "Resources.h"
class Shader
{
public:
	Shader(ComPtr < ID3D11Device2> device, const WCHAR* shaderReources);
	~Shader();
	void Update(ConstantBuffer* constantBuffer);
	void Render();
	ComPtr<ID3D11InputLayout>      m_inputLayout;                //   vertex input layout
  	ComPtr<ID3D11VertexShader>    m_vertexShader;               //   vertex shader
	ComPtr<ID3D11PixelShader>       m_pixelShader;                //   pixel shader
	ComPtr<ID3D11SamplerState>    m_sampler;                    //   texture sampler
	ComPtr<ID3D11Buffer>                m_constantBuffer;             // constant buffer resource
	ConstantBuffer                               m_constantBufferData;          // constant buffer resource data
private:
	ComPtr < ID3D11Device2>	m_Device;
};

