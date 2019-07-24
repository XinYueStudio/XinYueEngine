#include "MeshRender.h"
MeshRender::MeshRender()
{

}
MeshRender::~MeshRender()
{

}
bool MeshRender::Initialize(ID3D11Device2* device)
{
	 
	material = new Material();
	
	return material->Initialize(device);
}





void MeshRender::Update(ID3D11Device2*	device, ConstantBuffer  constantBuffer)
{
	material->Update(device,constantBuffer);
	
}

 

void MeshRender::Render(ID3D11DeviceContext2* deviceContext, int eyeindex)
{
	material->Render(deviceContext, eyeindex);
}
