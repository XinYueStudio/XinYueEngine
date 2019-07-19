#pragma once
#include "Material.h"
#include "Component.h"

class MeshRender:Component
{
public:
	MeshRender();
	~MeshRender();

	Material* material;

};