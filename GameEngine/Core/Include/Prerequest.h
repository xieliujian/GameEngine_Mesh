

#ifndef __Prerequest_h__
#define __Prerequest_h__

#define NAMESPACEBEGIN(x) namespace x {
#define NAMESPACEEND };

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef long int32;
typedef unsigned long uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

#define SAFE_DELETE_ARRAY(p) {if(p){delete []p; p = NULL;}}
#define SAFE_DELETE(p) {if(p){delete p; p = NULL;}}
#define SAFE_RELEASE(p) {if(p){p->Release(); p = NULL;}}

#define MDX_TAG(x) (DWORD)( (((DWORD)x&0x0000ff00)<<8) + (((DWORD)x&0x000000ff)<<24) + \
		(((DWORD)x&0x00ff0000)>>8) + (((DWORD)x&0xff000000)>>24) )

#define DC_TAG(x)  (DWORD)( (((DWORD)x&0x0000ff00)<<8) + (((DWORD)x&0x000000ff)<<24) + (((DWORD)x&0x00ff0000)>>8) + (((DWORD)x&0xff000000)>>24) )

#define FRAMEPERSEC 30.0f
#define ONEFRAMEINTERVAL (1000.0f / FRAMEPERSEC)

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#include <vector>
#include <map>
#include <list>
#include <stack>
#include <string>

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <mmsystem.h>
#include <commctrl.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;

inline DWORD HQ_TimeGetTime()
{
	return timeGetTime();
}

#endif