#pragma once

#include "Mesh.h"
#include "Component.h"

class MeshFilter:Component
{
public:
	MeshFilter(ComPtr < ID3D11Device2> device);
	~MeshFilter();
	
	Mesh* mesh;
private:
	ComPtr < ID3D11Device2>	m_Device;

};