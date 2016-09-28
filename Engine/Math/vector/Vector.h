#pragma once
#include "Math/MathStorage.h"
class Vector2 : public MathStorage<float, 2>
{
public:
	inline Vector2() {}
	inline Vector2(float x, float y)
	{
		setValue(0, x);
		setValue(1, y);
	}
	~Vector2() {}
	inline Vector2& operator=(const float& value)
	{
		setValue(0, value);
		setValue(1, value);
		return *this;
	}
};

class Vector3 : public MathStorage<float, 3>
{
public:
	inline Vector3() {}
	inline Vector3(float x, float y, float z)
	{
		setValue(0, x);
		setValue(1, y);
		setValue(2, z);
	}
	~Vector3() {}
	inline Vector3& operator=(const float& value)
	{
		setValue(0, value);
		setValue(1, value);
		setValue(2, value);
		return *this;
	}
};
class Vector4 : public MathStorage<float, 4>
{
public:
	inline Vector4() {}
	inline Vector4(float x, float y, float z, float w)
	{
		setValue(0, x);
		setValue(1, y);
		setValue(2, z);
		setValue(3, w);
	}
	~Vector4() {}
	inline Vector4& operator=(const float& value)
	{
		setValue(0, value);
		setValue(1, value);
		setValue(2, value);
		setValue(3, value);
		return *this;
	}
};
