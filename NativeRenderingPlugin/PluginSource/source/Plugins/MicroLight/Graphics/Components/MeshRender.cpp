#include "MeshRender.h"

MeshRender::MeshRender()
{
	material = new Material();
}



MeshRender::~MeshRender()
{
	delete(material);
}
