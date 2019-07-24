#pragma once
#ifndef _MeshRender_H_
#define _MeshRender_H_


#include "Material.h"

class Material;
class MeshRender
{
	 
public:
	MeshRender();
	
	~MeshRender();
	bool Initialize(ID3D11Device2* device);
	void Update(ID3D11Device2*	device, ConstantBuffer constantBuffer);
	void Render(ID3D11DeviceContext2* deviceContext, int eyeindex);
public:
	Material* material;

 
};

#endif