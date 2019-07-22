#pragma once


#include "RenderAPI.h"
#include "PlatformBase.h"

#if SUPPORT_D3D11
 
#include "MeshFilter.h"
#include "MeshRender.h"

 
class Quad
{
public:
	Quad(ComPtr < ID3D11Device2> device, string texturefile,float	frustumWidth, float	frustumHeight,Vector2 Pivot);
	Quad(ComPtr < ID3D11Device2> device, HANDLE texturehandle,float	frustumWidth, float	frustumHeight, Vector2 Pivot);
	~Quad();

	MeshFilter* m_MeshFilter;
	MeshRender*  m_MeshRender;
private:
	ComPtr < ID3D11Device2>	m_Device;

};

#endif