

#include "MyString.h"
#include <stdlib.h>

NAMESPACEBEGIN(GameEngine)

StringUtil::StringUtil()
{

}

StringUtil::~StringUtil()
{

}

void StringUtil::Trim(String &str, bool bLeft, bool bRight)
{
	static const String delims = " \t\r";
	if (bRight)
	{
		str.erase(str.find_last_not_of(delims) + 1); // trim right
	}

	if (bLeft)
	{
		str.erase(0, str.find_first_not_of(delims)); // trim left
	}
}

String StringUtil::Combine(String szSrc, String szSuffix, bool bUseName)
{
	char szDrive[MAX_PATH];
	char szDir[MAX_PATH];
	char szName[MAX_PATH];
	char szExt[MAX_PATH];
	_splitpath_s( (char*)szSrc.c_str(), szDrive, szDir, szName, szExt );

	char szSkeleton[MAX_PATH];

	if (bUseName)
	{
		sprintf_s(szSkeleton, "%s%s%s%s", szDrive, szDir, szName, szSuffix.c_str());
	}
	else
	{
		sprintf_s(szSkeleton, "%s%s%s", szDrive, szDir, szSuffix.c_str());
	}
		
	return szSkeleton;
}

bool StringUtil::Find(String src, String dest)
{
	bool bFind = (src.find(dest) != -1);
	if (bFind)
	{
		char szDrive[MAX_PATH];
		char szDir[MAX_PATH];
		char szName[MAX_PATH];
		char szExt[MAX_PATH];
		_splitpath_s( (char*)src.c_str(), szDrive, szDir, szName, szExt );
		if (!_stricmp(szExt, dest.c_str()))
		{
			return true;
		}
	}

	return false;
}

NAMESPACEEND