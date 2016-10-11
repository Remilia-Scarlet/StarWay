#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <assert.h>
#include <stdarg.h>
#include <cmath>
#include "Ash/RefCountPointer/RefCountPtr.h"

void DebugString(std::string format, ...);

#define TINY_DEFINE_PTR(CALSSNAME) class CALSSNAME;typedef RefCountPtr<CALSSNAME> CALSSNAME##Ptr;typedef WeakRefPtr<CALSSNAME> CALSSNAME##WeakPtr
#define TINY_BREAK_IF(CENTENCE) if(CENTENCE)break
#define TINY_SAFE_DELETE(PTR) do{if(PTR != nullptr){delete PTR;PTR = nullptr;}}while(0)
#define TINY_SAFE_RELEASE(PTR) do{if(PTR != nullptr){PTR->Release();PTR = nullptr;}}while(0)
#define TINY_FLOAT_EQUAL(_FLOATA_,_FLOATB_) (abs((_FLOATA_) - (_FLOATB_))<0.00000001f)

inline std::string FormatString(const char* format, ...)
{
	va_list _ArgList;
	va_start(_ArgList, format);
	char buffer[1024];
	vsprintf(buffer, format, _ArgList);
	va_end(_ArgList);
	return buffer;
}


#ifdef _DEBUG
inline void TinyAssert(bool condition, const char* format, ...)
{
	if (!condition)
	{
		va_list _ArgList;
		va_start(_ArgList, format);
		char buffer[1024];
		vsprintf(buffer, format, _ArgList);
		va_end(_ArgList);
		DebugString("Assert failed: %s", buffer);
		assert(false);
	}
}
inline void TinyAssert(bool condition) {assert(condition);}
#else
#define TinyAssert(...) 
#endif

typedef int64_t ObjectID;
const static ObjectID INVALID_OBJECT_ID = 0;

inline ObjectID genericObjectId() { static ObjectID id = INVALID_OBJECT_ID; return ++id; }