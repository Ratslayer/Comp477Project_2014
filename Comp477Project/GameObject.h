#pragma once
#include "Actor.h"
class GameObjectDesc
	: public ActorDesc
{
public:
	GameObjectDesc();
	GameObjectDesc(string data);
	string modelName;
	vec3 scale;
};
class GameObject :
	public Actor
{
public:
	GameObject();
	GameObject(string modelName);
	~GameObject();
	Model* pModel;
	void loadFromDesc(ActorDesc &desc);
	virtual void draw(Effect *effect);
	virtual void setScale(vec3 &v);
	virtual void setScale(real s);
protected: 
	vec3 scale;
};

