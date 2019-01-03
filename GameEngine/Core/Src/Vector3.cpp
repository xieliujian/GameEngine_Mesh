

#include "Vector3.h"

NAMESPACEBEGIN(GameEngine)

//--------------------------------
//-----------Vector3--------------
//--------------------------------
Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vector3::Normalize()
{
	float fFactor = 1.0f / Length();
	x *= fFactor;
	y *= fFactor;
	z *= fFactor;
}

float Vector3::Length()
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector3::Dot(const Vector3& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

Vector3 Vector3::Cross(const Vector3& rkVector) const
{
	return Vector3(
		y * rkVector.z - z * rkVector.y,
		z * rkVector.x - x * rkVector.z,
		x * rkVector.y - y * rkVector.x);
}

Vector3& Vector3::operator -()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

Vector3 Vector3::operator + (const Vector3 &v) const
{
	Vector3 r(x + v.x, y + v.y, z + v.z);
	return r;
}

Vector3 Vector3::operator - (const Vector3 &v) const
{
	Vector3 r(x - v.x, y - v.y, z - v.z);
	return r;
}

Vector3& Vector3::operator = (const Vector3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3& Vector3::operator += (const Vector3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator -= (const Vector3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 Vector3::operator * (float d) const
{
	Vector3 r(x * d, y * d, z * d);
	return r;
}

Vector3 Vector3::operator / (float m) const
{
	return Vector3(x / m, y / m, z / m);
}

NAMESPACEEND