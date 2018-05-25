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

	// Set location to parent node
	void setLocation(float deltaX, float deltaY, float deltaZ);

	// Set location to parent node
	void setLocation(const Vector3& position);

	// Get location to parent node
	const Vector3& getLocation();
	float getLocationX();
	float getLocationY();
	float getLocationZ();

	// Face to a point in parent coordinate system
	void faceToPoint(const Vector3& point);
	void faceToPoint(float x, float y, float z);


	// Set rotation. Rotate in z,x,y(roll,pitch,yaw) order. deltaX is rotation to local x axis.  deltaY is rotation to local y axis.  deltaZ is rotation to local z axis.
	void setEulerRotation(float deltaX, float deltaY, float deltaZ);
	void setEulerRotation(const Vector3& rotation);

	// Set rotation by quaternion 
	void setRotation(const Quaternion& rotation);

	// Get rotation, return quaternion. You can use Quaternion.toEuler() to get euler angle.
	const Quaternion& getRotation();
	
	// Get rotation, return euler angle rotating in zxy order
	Vector3 getEulerRotation();

	void setScale(float x, float y, float z);
	void setScale(const Vector3& scale);
	const Vector3& getScale();

	const Matrix4& getNodeToParentMatrix();
	const Matrix4& getParentToNodeMatrix();

	// Get y direction in parent coordinate
	Vector3 getUpDirection();

	// Get z direction in parent coordinate
	Vector3 getFrontDirection();

	// Get x direction in parent coordinate
	Vector3 getRightDirection();

	// convert a vector(direction) in parent coordinate into local coordinate
	Vector3 parentVectorToLocal(const Vector3& parentVector);

	// convert a point in parent coordinate into local coordinate
	Vector3 parentPointToLocal(const Vector3& parentPoint);

	virtual void render() override;
protected:
	virtual bool init();
	
	TransformComponent();

	Vector3 _location;

	//euler angle. Rotate in z,x,y order. 
	Quaternion _rotate; 

	Vector3 _scale;

	Matrix4 _nodeToParentMatrix;
	bool _nodeToParentMatrixDirty;

	Matrix4 _parentToNodeMatrix;
	bool _parentToNodeMatrixDirty;
};

