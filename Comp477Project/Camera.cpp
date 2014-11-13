#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	fovy = 90.0;
	nearZ = 1;
	farZ = 5000;
	at = vec3(0,0,0);
	setResolution(1024, 768);
}


Camera::~Camera()
{
}

mat44 Camera::getPerspective()
{
	return mat44::getPerspective(resolution.x / resolution.y, fovy, nearZ, farZ);
}

mat44 Camera::getView()
{
	return mat44::getView(position, at, up);
}

void Camera::lookAt(vec3 &v)
{
	at = v;
}

void Camera::setResolution(int width, int height)
{
	resolution.x = (float)width;
	resolution.y = (float)height;
}

void Camera::bind(Effect *effect)
{
	effect->getParam("mView") = getView();
	effect->getParam("mProjection") = getPerspective();
}

void Camera::loadFromDesc(ActorDesc &desc)
{
	Actor::loadFromDesc(desc);
	CameraDesc &cDesc = dynamic_cast<CameraDesc&>(desc);
	fovy = cDesc.fovy;
	nearZ = cDesc.nearZ;
	farZ = cDesc.farZ;
	at = cDesc.at;
}

CameraDesc::CameraDesc()
{

}

CameraDesc::CameraDesc(string data)
	:ActorDesc(data)
{
	fovy = bindings.getPrimitive<real>("fovy");
	nearZ = bindings.getPrimitive<real>("nearz");
	farZ = bindings.getPrimitive<real>("farz");
	at = bindings.getVec3("lookat");
}
