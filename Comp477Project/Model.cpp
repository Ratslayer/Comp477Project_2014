#include "stdafx.h"
#include "Model.h"
#include "AssetManager.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::loadFromFile(string fileName)
{
	ConfigScanner sc(fileName);
	string meshName = sc.params.get("Mesh");
	string materialName = sc.params.get("Material");

	pMesh = AssetManager::load<Mesh>(meshName);
	pMaterial = AssetManager::load<Material>(materialName);	
}

void Model::draw(Effect* effect)
{
	if (pMaterial)
		pMaterial->bind(effect);
	pMesh->draw(effect);
}
