#include "stdafx.h"
#include "Material.h"
#include "AssetManager.h"
#include "Effect.h"


Material::Material()
{
	pTexture = pNormalMap = pSpecularMap = NULL;
	shininess = 1;
}


Material::~Material()
{
}

void Material::loadFromFile(std::string fileName)
{
	Asset::loadFromFile(fileName);
	ConfigScanner sc(fileName);
	//load maps
	string textureName, normalName, specularName;

	textureName = sc.params.get("TextureMap");
	normalName = sc.params.get("NormalMap");
	specularName = sc.params.get("SpecularMap");

	pTexture = AssetManager::load<Texture>(textureName);
	pNormalMap = AssetManager::load<Texture>(normalName);
	pSpecularMap = AssetManager::load<Texture>(specularName);
	if (pNormalMap)
		pNormalMap->unitOffset = 1;
	if (pSpecularMap)
		pSpecularMap->unitOffset = 2;
}

void Material::bind(Effect *effect)
{
	if (pTexture)
		effect->getParam("t2Texture") = pTexture;
	if (pNormalMap)
		effect->getParam("t2NormalMap") = pNormalMap;
	if (pSpecularMap)
		effect->getParam("t2SpecularMap") = pSpecularMap;
}
