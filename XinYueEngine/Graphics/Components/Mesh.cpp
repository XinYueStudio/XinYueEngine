#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}


bool Mesh::Initialize( ID3D11Device2* device)
{
	 
	return true;
}


void Mesh::UpdateResources(ID3D11Device2* device)
{
	if (device == NULL)return;

	if (m_vertexBuffer)
		m_vertexBuffer.Reset();
	if (m_indexBuffer)
		m_indexBuffer.Reset();


	 if(VerticesCount>0&& Vertices!=nullptr)
	CreateVertexBuffer(device,VerticesCount,Vertices,m_vertexBuffer.GetAddressOf());
	 if (IndicesCount > 0 && Indices != nullptr)
	CreateIndexBuffer(device,IndicesCount, Indices, m_indexBuffer.GetAddressOf());

	m_indexCount = IndicesCount;
}


void Mesh::CreateVertexBuffer(_In_ ID3D11Device2* device,
	_In_ unsigned int numVertices,
	_In_ BasicVertex *vertexData,
	_Out_ ID3D11Buffer **vertexBuffer
)
{
	*vertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBufferInternal;

	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.ByteWidth = sizeof(BasicVertex) * numVertices;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexBufferData;
	VertexBufferData.pSysMem = vertexData;
	VertexBufferData.SysMemPitch = 0;
	VertexBufferData.SysMemSlicePitch = 0;

	ThrowIfFailed(
		device->CreateBuffer(
			&VertexBufferDesc,
			&VertexBufferData,
			&vertexBufferInternal
		)
	);


	*vertexBuffer = vertexBufferInternal.Detach();
}

void Mesh::CreateIndexBuffer(_In_ ID3D11Device2* device,
	_In_ unsigned int numIndices,
	_In_ unsigned short *indexData,
	_Out_ ID3D11Buffer **indexBuffer
)
{
	*indexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBufferInternal;

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.ByteWidth = sizeof(unsigned short) * numIndices;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA IndexBufferData;
	IndexBufferData.pSysMem = indexData;
	IndexBufferData.SysMemPitch = 0;
	IndexBufferData.SysMemSlicePitch = 0;

	ThrowIfFailed(
		device->CreateBuffer(
			&IndexBufferDesc,
			&IndexBufferData,
			&indexBufferInternal
		)
	);

	*indexBuffer = indexBufferInternal.Detach();
}