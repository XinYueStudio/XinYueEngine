#pragma once
#include "RenderAPI_D3D11.h"

class Mesh
{
public:
	Mesh();

	~Mesh();

	bool Initialize( ID3D11Device2* device);

	ComPtr<ID3D11Buffer>                m_vertexBuffer;               //   vertex buffer
	ComPtr<ID3D11Buffer>                m_indexBuffer;                //   index buffer
	unsigned int                                   m_indexCount;				   //   index count
 
	int VerticesCount;
	BasicVertex *Vertices;
	int IndicesCount;
	unsigned short *Indices;
 
	void UpdateResources(ID3D11Device2* device);


private:

	void CreateVertexBuffer(_In_ ID3D11Device2* device,
		_In_ unsigned int numVertices,
		_In_ BasicVertex *vertexData,
		_Out_ ID3D11Buffer **vertexBuffer);

	void CreateIndexBuffer(_In_ ID3D11Device2* device,
		_In_ unsigned int numIndices,
		_In_ unsigned short *indexData,
		_Out_ ID3D11Buffer **indexBuffer);

	  

};
 
