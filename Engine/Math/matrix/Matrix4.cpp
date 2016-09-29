#include "Math/precomp.h"
#include "Matrix4.h"



Matrix4::Matrix4()
{
}


Matrix4::~Matrix4()
{
}

Vector4& Matrix4::operator()(int index)
{
	TinyAssert(index < 4);
	return _m[index];
}

const Vector4& Matrix4::operator()(int index) const
{
	TinyAssert(index < 4);
	return _m[index];
}
