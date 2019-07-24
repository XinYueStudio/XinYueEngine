#include "Quad.h"
Quad::Quad()
{


}

Quad::~Quad()
{


}
bool Quad::InitializeFile(ID3D11Device2* device, string lefttexturefile, string righttexturefile, float	frustumWidth, float	frustumHeight, Vector2 Pivot, ConstantBuffer constantBuffer)
{
 
	m_MeshFilter = new MeshFilter();
	m_MeshFilter->Initialize(device);
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
	m_MeshFilter->mesh->UpdateResources(device);


	m_MeshRender = new MeshRender();
	m_MeshRender->Initialize(device);

	const	wstring ShaderFile = L"UIShader.hlsl";

	m_MeshRender->material->ShaderPass = new Shader();
	m_MeshRender->material->ShaderPass->Initialize(device, ShaderFile.c_str(), constantBuffer);
	m_MeshRender->material->ShaderPass->LeftRT = new Texture();
	m_MeshRender->material->ShaderPass->LeftRT->InitializeFile(device, lefttexturefile.c_str());
	m_MeshRender->material->ShaderPass->RightRT = new Texture();
	m_MeshRender->material->ShaderPass->RightRT->InitializeFile(device, righttexturefile.c_str());

 
	return true;
}

bool Quad::InitializeHandle(ID3D11Device2* device, HANDLE lefttexturehandle, HANDLE righttexturehandle, float frustumWidth, float frustumHeight, Vector2 Pivot, ConstantBuffer constantBuffer)
{
 
	m_MeshFilter = new MeshFilter();
	m_MeshFilter->Initialize(device);
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
	m_MeshFilter->mesh->UpdateResources(device);


	m_MeshRender = new MeshRender();
	m_MeshRender->Initialize(device);

	const	wstring ShaderFile = L"UIShader.hlsl";

	m_MeshRender->material->ShaderPass = new Shader();
	m_MeshRender->material->ShaderPass->Initialize(device, ShaderFile.c_str(), constantBuffer);
 	
	m_MeshRender->material->ShaderPass->LeftRT = new Texture();
	m_MeshRender->material->ShaderPass->LeftRT->InitializeHandle(device, lefttexturehandle);
	m_MeshRender->material->ShaderPass->RightRT = new Texture();
	m_MeshRender->material->ShaderPass->RightRT->InitializeHandle(device, righttexturehandle);


	return true;
}



void Quad::Update(ID3D11Device2* device, ConstantBuffer  constantBuffer)
{
	m_MeshFilter->Update(device,constantBuffer);
	m_MeshRender->Update(device,constantBuffer);
}

void Quad::Render(ID3D11DeviceContext2* deviceContext, int eyeindex)
{
	 
	m_MeshFilter->Render(deviceContext, eyeindex);
	m_MeshRender->Render(deviceContext, eyeindex);
	m_MeshFilter->DrawIndexed(deviceContext);
   
}
