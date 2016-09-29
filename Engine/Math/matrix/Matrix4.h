#pragma once
#include "Math\vector\Vector.h"
class Matrix4
{
public:
	Matrix4();
	~Matrix4();

	Vector4& operator()(int index);
	const Vector4& operator()(int index) const;
protected:
	Vector4 _m[4];
};

