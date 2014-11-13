#pragma once
#include "math.h"
class Vec4
{
public:
	Vec4();
	Vec4(real x, real y, real z, real w);
	~Vec4();
	real x, y, z, w;
	Vec4 operator *(Vec4 &v);
	real dot(Vec4 &v);
	quat toQuat();
};
typedef Vec4 vec4;

