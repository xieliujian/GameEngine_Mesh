

#include "Texture.h"

NAMESPACEBEGIN(GameEngine)

Texture::Texture(String szGroup, String szName)
	:Resource(szGroup, szName)
{
	m_nWidth = 0;
	m_nHeight = 0;
}

Texture::~Texture()
{

}

NAMESPACEEND