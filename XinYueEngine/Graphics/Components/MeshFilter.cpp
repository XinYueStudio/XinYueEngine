#include "MeshFilter.h"
MeshFilter::MeshFilter()
{

}

MeshFilter::~MeshFilter()
{

}
bool MeshFilter::Initialize(ID3D11Device2* device)
{
 
	mesh = new Mesh();
	mesh->Initialize(device);
	return true;
}



void MeshFilter::Update(ID3D11Device2* device, ConstantBuffer  constantBuffer)
{

}

void MeshFilter::Render(ID3D11DeviceContext2* deviceContext, int eyeindex)
{
 

	unsigned int stride;
	unsigned int offset;
	 
	// Set vertex buffer stride and offset.
	stride = sizeof(BasicVertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, mesh->m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(mesh->m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

void MeshFilter::DrawIndexed(ID3D11DeviceContext2* deviceContext)
{
 
	// Render the triangle.
	deviceContext->DrawIndexed(mesh->m_indexCount, 0, 0);
	 
}
