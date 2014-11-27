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
	virtual void loadFromFile(string fileName);
	string getClassName(){ return "Model"; }
	virtual void draw(Effect* effect);
};

