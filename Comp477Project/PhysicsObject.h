#pragma once
#include "GameObject.h"
class PhysicsDesc
	: public GameObjectDesc
{
public:
	PhysicsDesc();
	PhysicsDesc(string data);
	NxShapeType type;
};
class PhysicsObject :
	public GameObject
{
public:
	PhysicsObject();
	~PhysicsObject();
	virtual vec3 getPosition();
	virtual quat getRotation();
	void loadFromDesc(ActorDesc &desc);
	NxActor *actor;
};

