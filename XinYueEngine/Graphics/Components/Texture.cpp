
#include "Texture.h"

 
Texture::Texture()
{
}


 

Texture::~Texture()
{

}

bool Texture::InitializeFile(ID3D11Device2* device, const char * filename)
{
	m_Device = device;
	HRESULT result;

	string str = filename;
	wostringstream wstm;
	const ctype<wchar_t>& ctfacet =
		use_facet< ctype<wchar_t> >(wstm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
		wstm << ctfacet.widen(str[i]);

	wstring filenameSTR = wstm.str();

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(m_Device.Get(), filenameSTR.c_str(), NULL, NULL, &m_Texture, NULL);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

bool Texture::InitializeHandle(ID3D11Device2* device, HANDLE handle)
{
	return false;
}

ID3D11ShaderResourceView * Texture::GetTexture()
{
	return nullptr;
}

bool Texture::Shutdown()
{
	return false;
}
 