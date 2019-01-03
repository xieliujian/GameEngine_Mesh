

#include "MathTemp.h"

NAMESPACEBEGIN(GameEngine)

void Quat2Mat( Quaternion& q, Matrix4X4& mat )
{
	float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
	s = 2.0f / sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	xs = q.x * s; ys = q.y * s; zs = q.z * s;
	wx = q.w * xs; wy = q.w * ys; wz = q.w * zs;
	xx = q.x * xs; xy = q.x * ys; xz = q.x * zs;
	yy = q.y * ys; yz = q.y * zs; zz = q.z * zs;

	memset( mat.m, 0, sizeof(mat.m) );
	mat.m[0][0] = 1.0f - (yy + zz);
	mat.m[0][1] = xy - wz;
	mat.m[0][2] = xz + wy;
	mat.m[1][0] = xy + wz;
	mat.m[1][1] = 1.0f - (xx + zz);
	mat.m[1][2] = yz - wx;
	mat.m[2][0] = xz - wy;
	mat.m[2][1] = yz + wx;
	mat.m[2][2] = 1.0f - (xx + yy);
	mat.m[3][3] = 1.0f;
}

void MakeMatrix( Vector3& v, Quaternion& q, Matrix4X4& m )
{
	Quat2Mat( q, m );
		
	m.m[3][0] = v.x;
	m.m[3][1] = v.y;
	m.m[3][2] = v.z;
}

Vector3* TransformNormal(Vector3 *pOut, const Vector3 *vec3, const Matrix4X4 *mat)
{
	Vector3 tempVec3;

	tempVec3.x = vec3->x * mat->_11 + vec3->y * mat->_21 + vec3->z * mat->_31;
	tempVec3.y = vec3->x * mat->_12 + vec3->y * mat->_22 + vec3->z * mat->_32;
	tempVec3.z = vec3->x * mat->_13 + vec3->y * mat->_23 + vec3->z * mat->_33;

	*pOut = tempVec3;
	return pOut;
}

Matrix4X4* RotationAxis(Matrix4X4 *pOut, const Vector3 *vec3, float angle)
{
	memset(pOut, 0, sizeof(Matrix4X4));

	pOut->_44 = 1.0f;

	pOut->_11 = cos(angle) + vec3->x * vec3->x * (1 - cos(angle));
	pOut->_12 = vec3->z * sin(angle) + vec3->x * vec3->y * (1 - cos(angle));
	pOut->_13 = -vec3->y * sin(angle) + vec3->x * vec3->z * (1 - cos(angle));

	pOut->_21 = -vec3->z * sin(angle) + vec3->x * vec3->y * (1 - cos(angle));
	pOut->_22 = cos(angle) + vec3->y * vec3->y * (1 - cos(angle));
	pOut->_23 = vec3->x * sin(angle) + vec3->y * vec3->z * (1 - cos(angle));

	pOut->_31 = vec3->y * sin(angle) + vec3->x * vec3->z * (1 - cos(angle));
	pOut->_32 = -vec3->x * sin(angle) + vec3->y * vec3->z * (1 - cos(angle));
	pOut->_33 = cos(angle) + vec3->z * vec3->z * (1 - cos(angle));

	return pOut;
}

NAMESPACEEND