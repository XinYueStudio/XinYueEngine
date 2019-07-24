#pragma once

#ifndef _Shader_H_
#define _Shader_H_


#include "Texture.h"
class Texture;
 

 




class Shader
{
	 
public:
	Shader();
	~Shader();
	bool Initialize(ID3D11Device2* device, const WCHAR* shaderReources, ConstantBuffer  constantBuffer);
	void Update(ID3D11Device2* device,ConstantBuffer constantBuffer);
	void Render(ID3D11DeviceContext2* deviceContext, int eyeindex);
public:
	ComPtr<ID3D11InputLayout>      m_inputLayout;                //   vertex input layout
  	ComPtr<ID3D11VertexShader>    m_vertexShader;               //   vertex shader
	ComPtr<ID3D11PixelShader>       m_pixelShader;                //   pixel shader
	ComPtr<ID3D11SamplerState>    m_sampler;                    //   texture sampler
	ComPtr<ID3D11Buffer>                m_constantBuffer;             // constant buffer resource
	ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewLeft;
	Texture* LeftRT;
	Texture* RightRT;

};

#endif