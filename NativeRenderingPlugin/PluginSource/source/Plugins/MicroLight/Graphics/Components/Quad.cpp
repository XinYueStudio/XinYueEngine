#include "Quad.h"

Quad::Quad(ComPtr < ID3D11Device2> device,string texturefile,  float	frustumWidth, float	frustumHeight, Vector2 Pivot)
{
	m_Device=device;
	m_MeshFilter = new MeshFilter(device);

	BasicVertex axisVertices[] =
	{
		{ Vector3(-frustumWidth * 0.5f + Pivot.x,  frustumHeight *0.5f + Pivot.y, 0), Vector2(0.0f, 0.0f) },
		{ Vector3(frustumWidth *0.5f + Pivot.x, frustumHeight * 0.5f + Pivot.y, 0), Vector2(1.0f, 0.0f) },
		{ Vector3(frustumWidth *0.5f + Pivot.x, -frustumHeight * 0.5f + Pivot.y, 0), Vector2(1.0f, 1.0f) },
		{ Vector3(-frustumWidth * 0.5f + Pivot.x, -frustumHeight * 0.5f + Pivot.y, 0), Vector2(0.0f, 1.0f) },
	};

	unsigned short axisIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	m_MeshFilter->mesh->VerticesCount = ARRAYSIZE(axisVertices);
	m_MeshFilter->mesh->Vertices = axisVertices;
	m_MeshFilter->mesh->IndicesCount = ARRAYSIZE(axisIndices);
	m_MeshFilter->mesh->Indices = axisIndices;
	m_MeshFilter->mesh->UpdateResources();


	m_MeshRender = new MeshRender();
	Shader* ShaderPass = new Shader(device, "Texture.hlsl");
	m_MeshRender->material->ShaderPass = ShaderPass;
	Texture* MainTex = new Texture(device);
	MainTex->LoadTextureFromFile(texturefile);
	m_MeshRender->material->MainTex = MainTex;

}

Quad::Quad(ComPtr<ID3D11Device2> device, HANDLE texturehandle, float frustumWidth, float frustumHeight, Vector2 Pivot)
{
	m_Device = device;
	m_MeshFilter = new MeshFilter(device);

	BasicVertex axisVertices[] =
	{
		{ Vector3(-frustumWidth * 0.5f+ Pivot.x,  frustumHeight *0.5f + Pivot.y, 0), Vector2(0.0f, 0.0f) },
		{ Vector3(frustumWidth *0.5f + Pivot.x, frustumHeight * 0.5f + Pivot.y, 0), Vector2(1.0f, 0.0f) },
		{ Vector3(frustumWidth *0.5f + Pivot.x, -frustumHeight * 0.5f + Pivot.y, 0), Vector2(1.0f, 1.0f) },
		{ Vector3(-frustumWidth * 0.5f + Pivot.x, -frustumHeight * 0.5f + Pivot.y, 0), Vector2(0.0f, 1.0f) },
	};

	unsigned short axisIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	m_MeshFilter->mesh->VerticesCount = ARRAYSIZE(axisVertices);
	m_MeshFilter->mesh->Vertices = axisVertices;
	m_MeshFilter->mesh->IndicesCount = ARRAYSIZE(axisIndices);
	m_MeshFilter->mesh->Indices = axisIndices;
	m_MeshFilter->mesh->UpdateResources();


	m_MeshRender = new MeshRender();
	Shader* ShaderPass = new Shader(device, "Texture.hlsl");
	m_MeshRender->material->ShaderPass = ShaderPass;
	Texture* MainTex = new Texture(device);
	MainTex->LoadTextureFromShareHandle(texturehandle);
	m_MeshRender->material->MainTex = MainTex;
}

Quad::~Quad()
{
	delete(m_MeshFilter);
}
