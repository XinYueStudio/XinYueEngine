#include "Material.h"
Material::Material()
{

}
Material::~Material()
{

}
bool Material::Initialize(ID3D11Device2*	device)
{
 
  ShaderPass = new Shader();
  return true;
}


void Material::Update(ID3D11Device2*	device,ConstantBuffer  constantBuffer)
{
	ShaderPass->Update(device,constantBuffer);

	
}

 

void Material::Render(ID3D11DeviceContext2* deviceContext, int eyeindex)
{
	ShaderPass->Render(deviceContext, eyeindex);

	
}
