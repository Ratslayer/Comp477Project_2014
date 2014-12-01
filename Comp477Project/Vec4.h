#pragma once
#include "math.h"
class Vec4
{
public:
	Vec4();
	Vec4(vec3 v, real w);
	Vec4(real x, real y, real z, real w);
	~Vec4();
	real x, y, z, w;
	Vec4 operator *(Vec4 &v);
	Vec4 operator -(Vec4 &v);
	real dot(Vec4 &v);
	quat toQuat();
	vec3 toVec3();

};
typedef Vec4 vec4;
vec3 operator *(vec3 &v1, vec3 &v2);

