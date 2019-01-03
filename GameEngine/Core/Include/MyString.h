
#ifndef __OgreString_h__
#define __OgreString_h__

#include "Prerequest.h"

NAMESPACEBEGIN(GameEngine)

#if GAMEENGINE_WCHAR_T_STRINGS
	typedef std::wstring _StringBase;
#else
	typedef std::string String;
#endif

typedef std::vector<String> StringVector;

class StringUtil
{
public:
	StringUtil();
	~StringUtil();

	static void Trim(String &str, bool bLeft = true, bool bRight = true);
	static String Combine(String szSrc, String szSuffix, bool bUseName = false);
	static bool Find(String src, String dest);
};

NAMESPACEEND

#endif