#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <assert.h>
#include <stdarg.h>

void DebugString(std::string format, ...);

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
inline void TinyAssert(bool condition) 
{
	assert(condition);
}
#else
#define TinyAssert(...) do{}while(0)
#endif