#include "stdafx.h"
#include "Light.h"


Light::Light()
{
}


Light::~Light()
{
}

void Light::loadFromDesc(ActorDesc &desc)
{
	Actor::loadFromDesc(desc);
	LightDesc &lDesc = castDesc<LightDesc>(desc);
	color = lDesc.color;
	brightness = lDesc.brightness;
}

void Light::bind(Effect *effect)
{
	effect->safeSetParam<vec3>("f3LightColor", color);
	effect->safeSetParam<vec3>("f3LightPos", getPosition());
	effect->safeSetParam<float>("fBrightness", brightness);
	//effect->getParam("f3LightColor") = color;
	//effect->getParam("f3LightPos") = getPosition();
	//effect->getParam("fBrightness") = brightness;
}

LightDesc::LightDesc()
{
	color = vec3(1, 1, 1);
	brightness = 1;
}

LightDesc::LightDesc(string data)
	:ActorDesc(data)
{
	color = bindings.getVec3("color");
	brightness = bindings.getPrimitive<float>("brightness");
}
