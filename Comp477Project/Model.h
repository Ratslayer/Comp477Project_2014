#pragma once
#include "Actor.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Effect.h"
class Model :
	public Asset
{
public:
	Model();
	~Model();
	Mesh *pMesh;
	Material *pMaterial;
	void loadFromFile(string fileName);
	string getClassName(){ return "Model"; }
	void draw(Effect* effect);
};

