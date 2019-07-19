////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"







TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(ID3D11Device* device, const char* filename)
{
	HRESULT result;

	string str = filename;
	wostringstream wstm;
	const ctype<wchar_t>& ctfacet =
		use_facet< ctype<wchar_t> >(wstm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
		wstm << ctfacet.widen(str[i]);

	wstring filenameSTR = wstm.str();

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filenameSTR.c_str(), NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}