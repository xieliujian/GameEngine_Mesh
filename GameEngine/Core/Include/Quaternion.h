
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Prerequest.h"

NAMESPACEBEGIN(GameEngine)

class Quaternion
{
public:
	Quaternion(float x, float y, float z, float w);

	const Quaternion operator+ (const Quaternion &v) const;
	const Quaternion operator * (float d) const;

	float x;
	float y;
	float z;
	float w;
};

NAMESPACEEND

#endif
