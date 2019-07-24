#pragma once

 
#include "MeshFilter.h"
#include "MeshRender.h"

class MeshFilter;
class MeshRender;

class Quad
{
public:
	Quad();
	
	~Quad();

	bool InitializeFile(ID3D11Device2* device, string lefttexturefile, string righttexturefile, float	frustumWidth, float	frustumHeight, Vector2 Pivot,ConstantBuffer constantBuffer);
	bool InitializeHandle(ID3D11Device2* device, HANDLE lefttexturehandle, HANDLE righttexturehandle, float	frustumWidth, float	frustumHeight, Vector2 Pivot, ConstantBuffer constantBuffer);
	void Update(ID3D11Device2* device, ConstantBuffer constantBuffer);
	void Render(ID3D11DeviceContext2* deviceContext,int eyeindex);
public:
	MeshFilter* m_MeshFilter;
	MeshRender*  m_MeshRender;
 
};

 