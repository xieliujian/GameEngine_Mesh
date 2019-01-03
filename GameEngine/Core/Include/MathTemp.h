

#ifndef __OgreMath_h__
#define __OgreMath_h__

#include "Prerequest.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

NAMESPACEBEGIN(GameEngine)

void Quat2Mat(Quaternion& q, Matrix4X4& mat);
void MakeMatrix(Vector3& v, Quaternion& q, Matrix4X4& m);
Matrix4X4* RotationAxis(Matrix4X4 *pOut, const Vector3 *vec3, float angle);
Vector3* TransformNormal(Vector3 *pOut, const Vector3 *vec3, const Matrix4X4 *mat);

NAMESPACEEND

#endif