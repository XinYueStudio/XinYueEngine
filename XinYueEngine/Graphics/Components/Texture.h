#pragma once
#include "Resources.h"
class Texture
{
public:
	Texture(ComPtr < ID3D11Device2> device);
	~Texture();
	ComPtr<ID3D11ShaderResourceView>    m_texture;

	void LoadTextureFromFile(string File);
	void LoadTextureFromShareHandle(HANDLE handle);

private:
	ComPtr < ID3D11Device2>	m_Device;
};

