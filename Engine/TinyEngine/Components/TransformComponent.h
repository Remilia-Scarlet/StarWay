#pragma once
#include "BaseComponent.h"
#include "Math/vector/Vector.h"
#include "Math/quaternion/Quaternion.h"
#include "Math/matrix/Matrix.h"
#include "TinyEngine/Objects/Object.h"
#include "TinyEngine/ScriptManager/LuaManager.h"

TINY_DEFINE_PTR(TransformComponent);
class TransformComponent : public BaseComponent
{
public:
	static bool createLuaPrototype();
public:
	virtual ~TransformComponent();
public:
	static TransformComponentPtr create();
	LUA_CREATE_FUN_P0(TransformComponent);

	// Set location to parent node
	void setLocation(float deltaX, float deltaY, float deltaZ);

	// Set location to parent node
	void setLocation(const Vector3& position);
	LUA_MEMBER_FUN_P1R0_DECL(TransformComponent, setLocation, const Vector3&);

	// Get location to parent node
	const Vector3& getLocation();
	LUA_MEMBER_FUN_P0R1_DECL(TransformComponent, getLocation);
	float getLocationX();
	float getLocationY();
	float getLocationZ();

	// Face to a point in parent coordinate system
	void faceToPoint(const Vector3& point);
	LUA_MEMBER_FUN_P1R0_DECL(TransformComponent, faceToPoint, const Vector3&);
	void faceToPoint(float x, float y, float z);


	// Set rotation. Rotate in z,x,y(roll,pitch,yaw) order. deltaX is rotation to local x axis.  deltaY is rotation to local y axis.  deltaZ is rotation to local z axis.
	void setEulerRotation(float deltaX, float deltaY, float deltaZ);
	void setEulerRotation(const Vector3& rotation);
	LUA_MEMBER_FUN_P1R0_DECL(TransformComponent, setEulerRotation, const Vector3&);

	// Set rotation by quaternion 
	void setRotation(const Quaternion& rotation);

	// Get rotation, return quaternion. You can use Quaternion.toEuler() to get euler angle.
	const Quaternion& getRotation();
	
	// Get rotation, return euler angle rotating in zxy order
	Vector3 getEulerRotation();
	LUA_MEMBER_FUN_P0R1_DECL(TransformComponent, getEulerRotation);

	void setScale(float x, float y, float z);
	void setScale(const Vector3& scale);
	const Vector3& getScale();

	const Matrix4& getNodeToParentMatrix();
	const Matrix4& getParentToNodeMatrix();

	// Get y direction in parent coordinate
	Vector3 getUpDirection();
	LUA_MEMBER_FUN_P0R1_DECL(TransformComponent, getUpDirection);

	// Get z direction in parent coordinate
	Vector3 getFrontDirection();
	LUA_MEMBER_FUN_P0R1_DECL(TransformComponent, getFrontDirection);

	// Get x direction in parent coordinate
	Vector3 getRightDirection();
	LUA_MEMBER_FUN_P0R1_DECL(TransformComponent, getRightDirection);

	// convert a vector(direction) in parent coordinate into local coordinate
	Vector3 parentVectorToLocal(const Vector3& parentVector);
	LUA_MEMBER_FUN_P1R1_DECL(TransformComponent, parentVectorToLocal, const Vector3&);

	// convert a point in parent coordinate into local coordinate
	Vector3 parentPointToLocal(const Vector3& parentPoint);
	LUA_MEMBER_FUN_P1R1_DECL(TransformComponent, parentPointToLocal, const Vector3&);

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

