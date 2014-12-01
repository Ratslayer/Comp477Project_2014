#include "stdafx.h"
#include "Vec4.h"


Vec4::Vec4()
{
}

Vec4::Vec4(real x, real y, real z, real w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vec4::Vec4(vec3 v, real w)
{
	x = v.x;
	y = v.y;
	z = v.z;
	this->w = w;
}

Vec4::~Vec4()
{
}

real Vec4::dot(Vec4 &v)
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

quat Vec4::toQuat()
{
	quat result;
	result.w = w;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}

vec3 operator*(vec3 &v1, vec3 &v2)
{
	vec3 v;
	v.x = v1.x*v2.x;
	v.y = v1.y*v2.y;
	v.z = v1.z*v2.z;
	return v;
}
Vec4 Vec4::operator -(Vec4 &v)
{
	Vec4 r;
	r.x = x - v.x;
	r.y = y - v.y;
	r.z = z - v.z;
	r.w = w - v.w;
	return r;
}
vec3 Vec4::toVec3()
{
	return vec3(x, y, z);
}