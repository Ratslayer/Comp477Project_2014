#pragma once
#include "Actor.h"
class CameraDesc
	: public ActorDesc
{
public:
	CameraDesc();
	CameraDesc(string data);
	real fovy, nearZ, farZ;
	vec3 at;
};
class Camera :
	public Actor
{
public:
	Camera();
	~Camera();
	void loadFromDesc(ActorDesc &desc);
	mat44 getPerspective();
	mat44 getView();
	void lookAt(vec3 &v);
	real fovy;
	real nearZ, farZ;
	void setResolution(int width, int height);
	void bind(Effect *effect);
private:
	vec3 at;
	Vec2 resolution;
	
};

