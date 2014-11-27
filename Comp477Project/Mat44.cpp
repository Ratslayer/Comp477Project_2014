#include "stdafx.h"
#include "Mat44.h"


Mat44::Mat44()
{
	for (int i = 0; i < 16; i++)
		values[i] = 0;
}
Mat44::Mat44(mat33 &m)
{
	for (int i = 0; i < 3; i++)
	{
		vec3 row = m.getRow(i);
		values[4 * i] = row.x;
		values[4 * i + 1] = row.y;
		values[4 * i + 2] = row.z;
		values[4 * i + 3] = 0;
	}
	values[12] = values[13] = values[14] = 0;
	values[15] = 1;
}
Mat44::~Mat44()
{
}

Vec4 Mat44::getRow(int i)
{
	return vec4(values[4 * i], values[4 * i + 1], values[4 * i + 2], values[4 * i + 3]);
}

Vec4 Mat44::getCol(int i)
{
	return vec4(values[i], values[i + 4], values[i + 8], values[i + 12]);
}

Mat44 Mat44::operator *(Mat44 &m)
{
	Mat44 mat;
	for (int i = 0; i < 4; i++)
	{
		vec4 row = getRow(i);
		for (int j = 0; j < 4; j++)
		{
			vec4 col = m.getCol(j);

			mat.values[4 * i + j] = row.dot(col);
		}
	}
	return mat;
}

Mat44 Mat44::getPerspective(real aspectRatio, real fovyDegrees, real nearZ, real farZ)
{
	real fovy = NxMath::degToRad(fovyDegrees / 2);
	real f = 1 / tan(fovy);
	Mat44 mat = Mat44::getIdentity();
	mat.values[0] = f / aspectRatio;
	mat.values[5] = f;
	mat.values[10] = farZ / (nearZ - farZ);
	mat.values[11] = -1;
	mat.values[14] = farZ * nearZ / (nearZ - farZ);
	mat.values[15] = 0;
	return mat;
}

Mat44 Mat44::getTranslation(vec3 &v)
{
	Mat44 mat = getIdentity();
	mat.values[12] = -v.x;
	mat.values[13] = -v.y;
	mat.values[14] = -v.z;
	return mat;
}

Mat44 Mat44::getView(vec3 &eye, vec3 &at, vec3 &up)
{
	Mat44 rotation;
	vec3 zAxis = at - eye;
	vec3 xAxis = up.cross(zAxis);
	vec3 yAxis = xAxis.cross(zAxis);

	zAxis.normalize();
	xAxis.normalize();
	yAxis.normalize();

	rotation.values[0] = xAxis.x;
	rotation.values[1] = yAxis.x;
	rotation.values[2] = zAxis.x;

	rotation.values[4] = xAxis.y;
	rotation.values[5] = yAxis.y;
	rotation.values[6] = zAxis.y;

	rotation.values[8] = xAxis.z;
	rotation.values[9] = yAxis.z;
	rotation.values[10] = zAxis.z;

	rotation.values[12] = eye.dot(xAxis);
	rotation.values[13] = eye.dot(yAxis);
	rotation.values[14] = eye.dot(zAxis);
	rotation.values[15] = 1;

	//Mat44 translation = getTranslation(-eye);
	Mat44 result = rotation;
	return result;
}

Mat44 Mat44::getIdentity()
{
	Mat44 mat;
	mat.values[0] = 1;
	mat.values[5] = 1;
	mat.values[10] = 1;
	mat.values[15] = 1;
	return mat;
}

Mat44 Mat44::getScale(float s)
{
	Mat44 mat;
	mat.values[0] = mat.values[5] = mat.values[10] = s;
	mat.values[15] = 1;
	return mat;
}

Mat44 Mat44::getScale(vec3 &s)
{
	Mat44 mat;
	mat.values[0] = s.x;
	mat.values[5] = s.y;
	mat.values[10] = s.z;
	mat.values[15] = 1;
	return mat;
}

void Mat44::operator*=(Mat44 &m)
{
	*this = *this * m;
}

Mat44 Mat44::fromAxii(vec3 xAxis, vec3 yAxis, vec3 zAxis)
{
	mat44 m = mat44::getIdentity();
	m.values[0] = xAxis.x;
	m.values[1] = yAxis.x;
	m.values[2] = zAxis.x;

	m.values[4] = xAxis.y;
	m.values[5] = yAxis.y;
	m.values[6] = zAxis.y;

	m.values[8] = xAxis.z;
	m.values[9] = yAxis.z;
	m.values[10] = zAxis.z;
	return m;
}
