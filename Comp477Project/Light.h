#pragma once
#include "Actor.h"
class LightDesc : public ActorDesc
{
public:
	LightDesc();
	LightDesc(string data);
	vec3 color;
	float brightness;
};
class Light :
	public Actor
{
public:
	Light();
	~Light();
	void loadFromDesc(ActorDesc &desc);
	void bind(Effect *effect);
	vec3 color;
	float brightness;
};

