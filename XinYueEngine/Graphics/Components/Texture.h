#pragma once
#include "RenderAPI.h"
#include "PlatformBase.h"


#if SUPPORT_D3D11
#include "RenderAPI_D3D11.h"

class Texture
{
public:
	Texture();
	~Texture();
	ComPtr<ID3D11ShaderResourceView>    m_Texture;

	bool Initialize(ComPtr < ID3D11Device2> device,const char* filename);
	bool Initialize(ComPtr < ID3D11Device2> device, HANDLE handle);
	ID3D11ShaderResourceView* GetTexture();
	bool 	Shutdown();

private:
	ComPtr < ID3D11Device2>	m_Device;
};

#endif