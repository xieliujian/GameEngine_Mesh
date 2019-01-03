

#ifndef __Texture_h__
#define __Texture_h__

#include "Prerequest.h"
#include "Resource.h"

NAMESPACEBEGIN(GameEngine)

class Texture : public Resource
{
public:
	Texture(String szGroup, String szName);
	virtual ~Texture();

	virtual void* GetRealTex() { return NULL; }
protected:
	int m_nWidth;
	int m_nHeight;
};

NAMESPACEEND

#endif