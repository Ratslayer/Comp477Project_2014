#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "Effect.h"
class Material
	:public Asset
{
public:
	Material();
	~Material();
	Texture *pTexture, *pNormalMap, *pSpecularMap;
	float shininess;
	void loadFromFile(std::string fileName);
	string getClassName(){ return "Material"; }
	void bind(Effect *effect);
};

