#include "MeshFilter.h"

MeshFilter::MeshFilter(ComPtr<ID3D11Device2> device)
{
	m_Device = device;
	mesh = new Mesh(m_Device);
}

MeshFilter::~MeshFilter()
{
	delete(mesh);
}
