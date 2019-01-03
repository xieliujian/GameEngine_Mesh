
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "Prerequest.h"

NAMESPACEBEGIN(GameEngine)

class Vector3
{
public:
	Vector3(float x, float y, float z);
	Vector3(const Vector3& v);
	Vector3();

	void Normalize();
	float Length();
	float Dot(const Vector3& vec) const;
	Vector3 Cross(const Vector3& rkVector) const;

	Vector3& operator -();
	Vector3 operator + (const Vector3 &v) const;
	Vector3 operator - (const Vector3 &v) const;
	Vector3& operator = (const Vector3 &v);
	Vector3& operator += (const Vector3 &v);
	Vector3& operator -= (const Vector3 &v);
	Vector3 operator * (float d) const;
	Vector3 operator / (float d) const;

	float x;
	float y;
	float z;
};

NAMESPACEEND

#endif
