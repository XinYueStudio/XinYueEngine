#pragma once
#ifndef _MeshFilter_H_
#define _MeshFilter_H_


#include "Mesh.h"

class Mesh;
class MeshFilter
{
 

public:
	MeshFilter();

	~MeshFilter();
	bool Initialize( ID3D11Device2* device);
	void Update(ID3D11Device2* device, ConstantBuffer constantBuffer);
	void Render(ID3D11DeviceContext2* deviceContext, int eyeindex);
	void	DrawIndexed(ID3D11DeviceContext2* deviceContext);

public:
	Mesh* mesh;
 
};

#endif