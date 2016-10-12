#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <stdarg.h>
#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Math/MathDef.h"
#include "TinyEngine/Engine/TinyAssert.h"

void DebugString(std::string format, ...);

#define TINY_DEFINE_PTR(CALSSNAME) class CALSSNAME;typedef RefCountPtr<CALSSNAME> CALSSNAME##Ptr;typedef WeakRefPtr<CALSSNAME> CALSSNAME##WeakPtr
#define TINY_BREAK_IF(CENTENCE) if(CENTENCE)break
#define TINY_SAFE_DELETE(PTR) do{if(PTR != nullptr){delete PTR;PTR = nullptr;}}while(0)
#define TINY_SAFE_RELEASE(PTR) do{if(PTR != nullptr){PTR->Release();PTR = nullptr;}}while(0)

inline std::string FormatString(const char* format, ...)
{
	va_list _ArgList;
	va_start(_ArgList, format);
	char buffer[1024];
	vsprintf(buffer, format, _ArgList);
	va_end(_ArgList);
	return buffer;
}

typedef int64_t ObjectID;
const static ObjectID INVALID_OBJECT_ID = 0;

inline ObjectID genericObjectId() { static ObjectID id = INVALID_OBJECT_ID; return ++id; }