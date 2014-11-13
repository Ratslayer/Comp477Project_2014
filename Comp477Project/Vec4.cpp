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
