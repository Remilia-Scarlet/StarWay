#pragma once

#include <cmath>
#include <cfloat>

namespace DangoMath
{
	constexpr float PI = 3.14159265358979323846f;

	constexpr double DEG_TO_RAD = 0.0174532925199432957692369076848;
	constexpr double RAD_TO_DEG = 57.295779513082320876798154814105;

	constexpr float	TINY_FLOAT_EPSILON = 1e-5f;
	constexpr double TINY_DBL_EPSILON = DBL_EPSILON * 3;
	constexpr long double TINY_LDBL_EPSILON = LDBL_EPSILON * 3;

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
		return (abs((a) - (b))<TINY_FLOAT_EPSILON);
	}

	template<>
	inline bool isEqual<double>(const double& a, const double& b)
	{
		return (abs((a) - (b))<TINY_DBL_EPSILON);
	}

	template<class ValueType>
	inline bool isGreater(const ValueType &a, const ValueType& b)
	{
		return a > b;
	}

	template<>
	inline bool isGreater<float>(const float& a, const float& b)
	{
		return ((a) >= (b) + (TINY_FLOAT_EPSILON));
	}

	template<>
	inline bool isGreater<double>(const double& a, const double& b)
	{
		return ((a) >= (b) + (TINY_DBL_EPSILON));
	}

	template<class ValueType>
	inline bool isGreaterOrEqual(const ValueType &a, const ValueType& b)
	{
		return a >= b;
	}

	template<>
	inline bool isGreaterOrEqual<float>(const float& a, const float& b)
	{
		return ((a) > (b) - (TINY_FLOAT_EPSILON));
	}

	template<>
	inline bool isGreaterOrEqual<double>(const double& a, const double& b)
	{
		return ((a) > (b) - (TINY_DBL_EPSILON));
	}


	template<class ValueType>
	inline bool isLess(const ValueType &a, const ValueType& b)
	{
		return a < b;
	}

	template<>
	inline bool isLess<float>(const float& a, const float& b)
	{
		return ((a) <= (b) - (TINY_FLOAT_EPSILON));
	}

	template<>
	inline bool isLess<double>(const double& a, const double& b)
	{
		return ((a) <= (b) - (TINY_DBL_EPSILON));
	}

	template<class ValueType>
	inline bool isLessOrEuqal(const ValueType &a, const ValueType& b)
	{
		return a < b;
	}

	template<>
	inline bool isLessOrEuqal<float>(const float& a, const float& b)
	{
		return ((a) < (b) + (TINY_FLOAT_EPSILON));
	}

	template<>
	inline bool isLessOrEuqal<double>(const double& a, const double& b)
	{
		return ((a) < (b) + (TINY_DBL_EPSILON));
	}
}