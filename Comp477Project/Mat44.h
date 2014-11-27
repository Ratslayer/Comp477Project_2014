#pragma once
#include "math.h"
class Mat44
{
public:
	Mat44();
	Mat44(mat33&);
	~Mat44();
	float values[16];
	Vec4 getRow(int i);
	Vec4 getCol(int i);
	Mat44 operator *(Mat44 &m);
	void operator *=(Mat44 &m);
	//Mat44& operator =(mat33 &m);
	//static methods
	static Mat44 fromAxii(vec3 xAxis, vec3 yAxis, vec3 zAxis);
	static Mat44 getPerspective(real aspectRatio, real fovyDegrees, real nearZ, real farZ);
	static Mat44 getTranslation(vec3 &v);
	static Mat44 getView(vec3 &eye, vec3 &at, vec3 &up);
	static Mat44 getScale(float s);
	static Mat44 getScale(vec3 &s);
	static Mat44 getIdentity();
	//static Mat44 getRotation(quat& rotation);

};
typedef Mat44 mat44;

