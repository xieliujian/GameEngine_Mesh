
#include "Quaternion.h"

NAMESPACEBEGIN(GameEngine)

Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

const Quaternion Quaternion::operator * (float d) const
{
	Quaternion r(x * d, y * d, z * d, w * d);
	return r;
}

const Quaternion Quaternion::operator + (const Quaternion &v) const
{
	Quaternion r(x + v.x, y + v.y, z + v.z, w + v.w);
	return r;
}


NAMESPACEEND