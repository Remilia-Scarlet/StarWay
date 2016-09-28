#pragma once
#include "Math/MathStorage.h"
class Vector2 : public MathStorage<float, 2>
{
public:
	inline Vector2() {}
	inline Vector2(float x, float y);
	~Vector2() {}
	inline Vector2& operator=(const float& value);
};

class Vector3 : public MathStorage<float, 3>
{
public:
	inline Vector3() {}
	inline Vector3(float x, float y, float z);
	~Vector3() {}
	inline Vector3& operator=(const float& value);
};
class Vector4 : public MathStorage<float, 4>
{
public:
	inline Vector4() {}
	inline Vector4(float x, float y, float z, float w);
	~Vector4() {}
	inline Vector4& operator=(const float& value);
};
