#pragma once
#ifndef _Material_H_
#define _Material_H_
 
#include "Shader.h"

class Shader;

class Material
{
 
public:
	Material();

	~Material();
	bool Initialize( ID3D11Device2*	device);
	void Update(ID3D11Device2*	device, ConstantBuffer constantBuffer);
	void Render(ID3D11DeviceContext2* deviceContext, int eyeindex);
public:
	Shader* ShaderPass;
   
 
};
#endif