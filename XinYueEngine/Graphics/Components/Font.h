#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Font.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Font_H_
#define _Font_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Font
////////////////////////////////////////////////////////////////////////////////
class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	bool Initialize(ID3D11Device2*, const char*, const char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, const char*, float, float);

private:
	bool LoadFontData(const char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device2*, const char*);
	void ReleaseTexture();

private:
	FontType* m_Font;
	Texture* m_Texture;
};

#endif