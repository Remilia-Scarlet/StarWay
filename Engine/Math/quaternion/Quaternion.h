#pragma once
#include "Math\vector\Vector.h"
#include "Math\matrix\Matrix4.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(const Vector3& eulerAngles);
	Quaternion(float xRotate, float yRotate, float zRotate);
	Quaternion(const Matrix4& rotationMatrix);
	Quaternion(const Quaternion& other);
	void setRotation(const Vector3& eulerAngles);
	void setRotation(float xRotate, float yRotate, float zRotate);
	void setRotation(const Matrix4& rotationMatrix);
	virtual ~Quaternion();
	float W() const { return _w; }
	float X() const { return _x; }
	float Y() const { return _y; }
	float Z() const { return _z; }
protected:
	float _w;
	float _x;
	float _y;
	float _z;
};

