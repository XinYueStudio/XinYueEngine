#include "Texture.h"



Texture::Texture(ComPtr < ID3D11Device2> device)
{
	m_Device = device;
}


Texture::~Texture()
{

}

void Texture::LoadTextureFromFile(string File)
{

}

void Texture::LoadTextureFromShareHandle(HANDLE handle)
{
	if (m_Device)
	{

	}
}
