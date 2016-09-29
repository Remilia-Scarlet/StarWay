#pragma once
#include "BaseComponent.h"
#include "Math/vector/Vector.h"
#include "Math/quaternion/Quaternion.h"
#include "Math/matrix/Matrix.h"
#include "TinyEngine/Objects/Object.h"

TINY_DEFINE_PTR(TransformComponent);
class TransformComponent : public BaseComponent
{
public:
	virtual ~TransformComponent();
public:
	static TransformComponentPtr create();

	void setLocation(const Vector3& position) { _nodeToParentMatrixDirty = true; _location = position; }
	Vector3 getLocation() { return _location; }

	virtual void faceToDir(const Vector3& lookAt);
	virtual void faceToPoint(const Vector3& point);

	void setRotation(float deltaX, float deltaY, float deltaZ);
	void setRotation(const Quaternion& rotation);
	void setRotationX(float deltaX);
	void setRotationY(float deltaY);
	void setRotationZ(float deltaZ);
	const Quaternion& getRotation();
	float getRotationX();
	float getRotationY();
	float getRotationZ();

	void setScale(float x, float y, float z) { _scale(0) = x; _scale(1) = y; _scale(2) = z; }
	void setScale(const Vector3& scale) { _scale = scale; }
	const Vector3& getScale() { return _scale; }

	const Matrix4& getWorldMatrix();
	Vector3 getUpDirection();//return z direction
	Vector3 getFrontDirection();//return y direction

	virtual void render() override;
protected:
	virtual bool init();
	
	explicit TransformComponent();
	Vector3 _location;
	Quaternion _rotate;
	Vector3 _scale;
	bool _nodeToParentMatrixDirty;
	Matrix4 _nodeToParentMatrix;
};

