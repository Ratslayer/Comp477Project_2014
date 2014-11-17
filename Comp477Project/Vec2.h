#pragma once
#include "math.h"
class Vec2
{
public:
	Vec2();
	Vec2(real x, real y);
	~Vec2();
	real x, y;
	Vec2 operator -(Vec2 &v);
	void operator +=(Vec2 &v);
	void operator /=(float f);
};
typedef Vec2 vec2;

