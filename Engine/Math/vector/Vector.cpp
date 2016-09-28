#include "Math/precomp.h"
#include "Vector.h"

Vector2::Vector2(float a, float b)
{
	setValue(0, a);
	setValue(1, b);
}

Vector2& Vector2::operator=(const float& value)
{
	setValue(0, value);
	setValue(1, value);
	return *this;
}

Vector3::Vector3(float x, float y, float z)
{
	setValue(0, x);
	setValue(1, y);
	setValue(2, z);
}

Vector3& Vector3::operator=(const float& value)
{
	setValue(0, value);
	setValue(1, value);
	setValue(2, value);
	return *this;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	setValue(0, x);
	setValue(1, y);
	setValue(2, z);
	setValue(3, w);
}

Vector4& Vector4::operator=(const float& value)
{
	setValue(0, value);
	setValue(1, value);
	setValue(2, value);
	setValue(3, value);
	return *this;
}
