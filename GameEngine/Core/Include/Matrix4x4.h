
#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

#include "Prerequest.h"
#include "Vector3.h"
#include "Quaternion.h"

NAMESPACEBEGIN(GameEngine)

class Matrix4X4
{
public:
	Matrix4X4();
	Matrix4X4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	Matrix4X4 Inverse();
	Matrix4X4 operator * (Matrix4X4 &m2);
	Matrix4X4 operator *= (Matrix4X4 m2);
	Vector3 operator * (const Vector3& v) const;

	void Translation(const Vector3& tr);
	void Scale(const Vector3& sc);
	void QuaternionRotate(const Quaternion& q);
	void Transpose();

	void Unit();
	void Zero();

	static Matrix4X4* RotationAxis(Matrix4X4 *pOut, const Vector3 *vec3, float angle);
	static const Matrix4X4 NewTranslation(const Vector3& tr);
	static const Matrix4X4 NewQuatRotate(const Quaternion& qr);
	static const Matrix4X4 NewScale(const Vector3& sc);
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

NAMESPACEEND

#endif