#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Front.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Front_H_
#define _Front_H_


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
// Class name: Front
////////////////////////////////////////////////////////////////////////////////
class Front
{
private:
	struct FrontType
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
	Front();
	Front(const Front&);
	~Front();

	bool Initialize(ID3D11Device*, const char*, const char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, const char*, float, float);

private:
	bool LoadFrontData(const char*);
	void ReleaseFrontData();
	bool LoadTexture(ID3D11Device*, const char*);
	void ReleaseTexture();

private:
	FrontType* m_Front;
	Texture* m_Texture;
};

#endif