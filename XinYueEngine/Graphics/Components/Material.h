#pragma once
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material();
	~Material();

	Shader* ShaderPass;
	Texture* MainTex;

};
