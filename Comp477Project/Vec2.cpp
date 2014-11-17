#include "stdafx.h"
#include "Vec2.h"


Vec2::Vec2()
{
	x = y = 0;
}

Vec2::Vec2(real x, real y)
{
	this->x = x;
	this->y = y;
}


Vec2::~Vec2()
{
}

void Vec2::operator+=(Vec2 &v)
{
	x += v.x;
	y += v.y;
}

void Vec2::operator/=(float f)
{
	x /= f;
	y /= f;
}

Vec2 Vec2::operator-(Vec2 &v)
{
	vec2 r;
	r.x = x - v.x;
	r.y = y - v.y;
	return r;
}
