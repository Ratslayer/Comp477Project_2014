#include "stdafx.h"
#include "Actor.h"
#include "AssetManager.h"


Actor::Actor()
{
	rotation.setw(1);
	forward = vec3(0, 0, 1);
	up = vec3(0, 1, 0);

}
Actor::~Actor()
{
}

vec3 Actor::getPosition()
{
	return position;
}

vec3 Actor::getUp()
{
	return getRotatedVec3(up);
}

vec3 Actor::getForward()
{
	return getRotatedVec3(forward);
}

quat Actor::getRotation()
{
	return rotation;
}

void Actor::setPosition(vec3 &v)
{
	position = v;
}

void Actor::setUp(vec3 &v)
{
	up = v;
}

void Actor::setForward(vec3 &v)
{
	forward = v;
}

void Actor::setRotation(quat &q)
{
	rotation = q;
}

void Actor::addPosition(vec3 &v)
{
	position += v;
}

void Actor::addVelocity(vec3 &v)
{
	velocity += v;
}

void Actor::setVelocity(vec3&v)
{
	velocity = v;
}

void Actor::setAcceleration(vec3&v)
{
	acceleration = v;
}

void Actor::addAcceleration(vec3&v)
{
	acceleration += v;
}

void Actor::addRotation(quat&q)
{
	rotation *= q;
}

void Actor::Update(real dTime)
{
	velocity += acceleration * dTime;
	position += velocity * dTime;
	//rotation += angularVelocity*dTime;
	//angularVelocity.
}

vec3 Actor::getRotatedVec3(vec3 &v)
{
	vec3 newV = v;
	rotation.rotate(newV);
	return newV;
}

void Actor::loadFromDesc(ActorDesc &desc)
{
	name		= desc.name;
	position	= desc.position;
	velocity	= desc.velocity;
	rotation	= desc.rotation;
}
ActorDesc::ActorDesc()
{

}
ActorDesc::ActorDesc(string data)
{
	bindings = BindingCollection(data);
	position = bindings.getVec3("Position");
	velocity = bindings.getVec3("Velocity");
	rotation = bindings.getQuat("Rotation");
	name = bindings.get("name");
}
