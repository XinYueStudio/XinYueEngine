#pragma once
#include "RenderAPI_D3D11.h"

class Texture
{
public:
	Texture();
	~Texture();
	
	bool InitializeFile(ID3D11Device2* device,const char* filename);
	bool InitializeHandle(ID3D11Device2* device, HANDLE handle);
	ID3D11ShaderResourceView* GetTexture();
	bool 	Shutdown();

public:
	ID3D11ShaderResourceView*    m_Texture;

private:
	ComPtr < ID3D11Device2>	m_Device;
};

 