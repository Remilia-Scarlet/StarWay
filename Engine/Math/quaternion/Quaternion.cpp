#include "Math/precomp.h"
#include "Quaternion.h"



Quaternion::Quaternion()
	:_w(0),_x(0),_y(0),_z(0)
{
}


Quaternion::Quaternion(const Vector3& eulerAngles)
{

}

Quaternion::Quaternion(float xRotate, float yRotate, float zRotate)
{

}

Quaternion::Quaternion(const Matrix4& rotationMatrix)
{

}

Quaternion::Quaternion(const Quaternion& other)
{
	_x = other._x;
	_y = other._y;
	_z = other._z;
	_w = other._w;
}

void Quaternion::setRotation(const Vector3& eulerAngles)
{

}

void Quaternion::setRotation(float xRotate, float yRotate, float zRotate)
{

}

void Quaternion::setRotation(const Matrix4& rotationMatrix)
{

}

Quaternion::~Quaternion()
{
}
