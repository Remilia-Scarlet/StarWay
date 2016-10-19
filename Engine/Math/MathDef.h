#pragma once

#define _USE_MATH_DEFINES   // Make math.h define M_PI
#include <math.h>
#include <float.h>

#ifndef M_PI
	#define M_PI       3.14159265358979323846   // pi
#endif

const double DEG_TO_RAD = 0.0174532925199432957692369076848;
const double RAD_TO_DEG = 57.295779513082320876798154814105;

const float	TINY_FLOAT_EPSILON = 1e-5f;
const double TINY_DBL_EPSILON = DBL_EPSILON * 3;
const long double TINY_LDBL_EPSILON = LDBL_EPSILON * 3;

#define TINY_FLOAT_EQUAL(_A_,_B_) (abs((_A_) - (_B_))<TINY_FLOAT_EPSILON)
#define TINY_DOUBLE_EQUAL(_A_,_B_) (abs((_A_) - (_B_))<TINY_DBL_EPSILON)
#define TINY_LONG_DOUBLE_EQUAL(_A_,_B_) (abs((_A_) - (_B_))<TINY_LDBL_EPSILON)

#define TINY_FLOAT_GREATER(_A_,_B_) ((_A_) >= (_B_) + (TINY_FLOAT_EPSILON))
#define TINY_DOUBLE_GREATER(_A_,_B_) ((_A_) >= (_B_) + (TINY_DBL_EPSILON))
#define TINY_LONG_DOUBLE_GREATER(_A_,_B_) ((_A_) >= (_B_) + (TINY_LDBL_EPSILON))

#define TINY_FLOAT_LESS(_A_,_B_) ((_A_) <= (_B_) - (TINY_FLOAT_EPSILON))
#define TINY_DOUBLE_LESS(_A_,_B_) ((_A_) <= (_B_) - (TINY_DBL_EPSILON))
#define TINY_LONG_DOUBLE_LESS(_A_,_B_) ((_A_) <= (_B_) - (TINY_LDBL_EPSILON))


template<class ValueType>
inline ValueType degToRad(const ValueType& degree)
{
	return degree * (ValueType)DEG_TO_RAD;
}

template<class ValueType>
inline ValueType radToDeg(const ValueType& radian)
{
	return radian * (ValueType)RAD_TO_DEG;
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

template<class ValueType>
inline bool isGreater(const ValueType &a, const ValueType& b)
{
	return a > b;
}

template<>
inline bool isGreater<float>(const float& a, const float& b)
{
	return TINY_FLOAT_GREATER(a, b);
}

template<>
inline bool isGreater<double>(const double& a, const double& b)
{
	return TINY_DOUBLE_GREATER(a, b);
}

template<class ValueType>
inline bool isLess(const ValueType &a, const ValueType& b)
{
	return a < b;
}

template<>
inline bool isLess<float>(const float& a, const float& b)
{
	return TINY_FLOAT_LESS(a, b);
}

template<>
inline bool isLess<double>(const double& a, const double& b)
{
	return TINY_DOUBLE_LESS(a, b);
}
