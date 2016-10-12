#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <assert.h>
#include <stdarg.h>
#define _USE_MATH_DEFINES   // Make math.h define M_PI
#include <math.h>
#include "Ash/RefCountPointer/RefCountPtr.h"

void DebugString(std::string format, ...);

#define TINY_DEFINE_PTR(CALSSNAME) class CALSSNAME;typedef RefCountPtr<CALSSNAME> CALSSNAME##Ptr;typedef WeakRefPtr<CALSSNAME> CALSSNAME##WeakPtr
#define TINY_BREAK_IF(CENTENCE) if(CENTENCE)break
#define TINY_SAFE_DELETE(PTR) do{if(PTR != nullptr){delete PTR;PTR = nullptr;}}while(0)
#define TINY_SAFE_RELEASE(PTR) do{if(PTR != nullptr){PTR->Release();PTR = nullptr;}}while(0)
#define TINY_FLOAT_EQUAL(_A_,_B_) (abs((_A_) - (_B_))<1e-5f)
#define TINY_DOUBLE_EQUAL(_A_,_B_) (abs((_A_) - (_B_))<1e-12)
#define TINY_LONG_DOUBLE_EQUAL(_A_,_B_) (abs((_A_) - (_B_))<1e-15l)

template<class ValueType>
inline ValueType degreeToRadian(const ValueType& degree)
{
	return degree / (ValueType)180 * (ValueType)M_PI;
}

template<class ValueType>
inline ValueType radianToDegree(const ValueType& radian)
{
	return radian / (ValueType)M_PI * (ValueType)180;
}

template<class ValueType>
inline bool isEqual(const ValueType &a, const ValueType& b)
{
	return a == b;
}

template<>
inline bool isEqual<float>(const float& a, const float& b)
{
	return TINY_FLOAT_EQUAL(a, b);
}

template<>
inline bool isEqual<double>(const double& a, const double& b)
{
	return TINY_DOUBLE_EQUAL(a, b);
}

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