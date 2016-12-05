#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <assert.h>
#include <stdarg.h>
#include <memory>

void DebugString(std::string format, ...);

#ifdef _DEBUG
#define TinyAssert(EXPRESSION, FORMAT, ...)\
do{\
	if (!(EXPRESSION))\
	{\
		DebugString(FORMAT,__VA_ARGS__);\
		assert(EXPRESSION);\
	}\
}while(0)

#define TinyAssert(EXPRESSION) \
do{\
	assert(EXPRESSION);\
}while(0)
#else
#define TinyAssert(...) do{}while(0)
#endif