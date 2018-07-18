#include "TinyEngine/precomp.h"
#include "TransformComponent.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include "TinyEngine\Engine\Engine.h"
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\GraphicMgr.h"
#include <DirectXMath.h>


bool TransformComponent::init()
{
	do 
	{

		return true;
	} while (0);
	return false;

}

TransformComponent::TransformComponent()
	:BaseComponent(TO_STRING(TransformComponent))
	, _scale{ 1.f,1.f,1.f }
	, _nodeToParentMatrixDirty(true)
	, _parentToNodeMatrixDirty(true)
{
}


TransformComponent::~TransformComponent()
{
}

TransformComponentPtr TransformComponent::create()
{
	TransformComponent* ret = new TransformComponent();
	if (!ret || !ret->init())
		TINY_SAFE_DELETE(ret);
	return TransformComponentPtr(ret);
}

void TransformComponent::setLocation(const Vector3& position)
{
	setLocation(position.X(), position.Y(), position.Z());
}

void TransformComponent::setLocation(float deltaX, float deltaY, float deltaZ)
{
	_location.X() = deltaX;
	_location.Y() = deltaY;
	_location.Z() = deltaZ;
	_nodeToParentMatrixDirty = _parentToNodeMatrixDirty = true;
}

const Vector3& TransformComponent::getLocation()
{
	return _location;
}

float TransformComponent::getLocationX()
{
	return _location.X();
}

float TransformComponent::getLocationY()
{
	return _location.Y();
}

float TransformComponent::getLocationZ()
{
	return _location.Z();
}


void TransformComponent::faceToPoint(const Vector3& point)
{
	if (point == _location)
		return;

	Vector3 vecThis2Point = point - _location;
	_rotate = Quaternion(Vector3(0,0,1), vecThis2Point);
}

void TransformComponent::faceToPoint(float x, float y, float z)
{
	faceToPoint({ x,y,z });
}

void TransformComponent::setEulerRotation(float deltaX, float deltaY, float deltaZ)
{
	setRotation(Quaternion(deltaX, deltaY, deltaZ));
}

void TransformComponent::setRotation(const Quaternion& rotation)
{
	_rotate = rotation;
	_nodeToParentMatrixDirty = _parentToNodeMatrixDirty = true;
}

void TransformComponent::setEulerRotation(const Vector3& rotation)
{
	setEulerRotation(rotation.X(), rotation.Y(), rotation.Z());
}

const Quaternion& TransformComponent::getRotation()
{
	return _rotate;
}

Vector3 TransformComponent::getEulerRotation()
{
	return getRotation().toEularAngle();
}

void TransformComponent::setScale(float x, float y, float z)
{
	_scale.X() = x;
	_scale.Y() = y;
	_scale.Z() = z;
	_nodeToParentMatrixDirty = _parentToNodeMatrixDirty = true;
}

void TransformComponent::setScale(const Vector3& scale)
{
	setScale(scale.X(), scale.Y(), scale.Z());
}

const Vector3& TransformComponent::getScale()
{
	return _scale;
}

const Matrix4& TransformComponent::getNodeToParentMatrix()
{
	if (_nodeToParentMatrixDirty)
	{
		TinyAssert(_owner.isValid());

		Matrix4 rotationMatrix = _rotate.toRotationMatrix();
		Matrix4 translation = CreateTranslaionMatrixFromVector(_location); 
		Matrix4 scaling = CreateScalingMatrix(_scale.X(), _scale.Y(), _scale.Z());

		_nodeToParentMatrix = rotationMatrix * translation * scaling;
		_nodeToParentMatrixDirty = false;
	}
	return _nodeToParentMatrix;
}

const Matrix4& TransformComponent::getParentToNodeMatrix()
{
	if (_parentToNodeMatrixDirty)
	{
		TinyAssert(_owner.isValid());

		Matrix4 rotationMatrix = _rotate.conjugate().toRotationMatrix();
		Matrix4 translation = CreateTranslaionMatrixFromVector(Vector3() - _location);
		Matrix4 scaling = CreateScalingMatrix(1.0f / _scale.X(), 1.0f / _scale.Y(), 1.0f / _scale.Z());

		_parentToNodeMatrix = scaling * translation * rotationMatrix;
		_parentToNodeMatrixDirty = false;
	}
	return _parentToNodeMatrix;
}

Vector3 TransformComponent::getUpDirection()
{
	return getNodeToParentMatrix().getRow(1).subVecter<0,3>();
}

Vector3 TransformComponent::getFrontDirection()
{
	return getNodeToParentMatrix().getRow(2).subVecter<0, 3>();
}

Vector3 TransformComponent::getRightDirection()
{
	return getNodeToParentMatrix().getRow(0).subVecter<0, 3>();
}

Vector3 TransformComponent::parentVectorToLocal(const Vector3& parentVector)
{
	MatrixStorage<float, 1, 4> vecMatrix = { parentVector.X(), parentVector.Y(), parentVector.Z(), 0 };
	vecMatrix.dotInPlace(getParentToNodeMatrix());
	return vecMatrix.getPartOfMatix<1, 3>(0, 0).getData();
}

Vector3 TransformComponent::parentPointToLocal(const Vector3& parentPoint)
{
	MatrixStorage<float, 1, 4> vecMatrix = { parentPoint.X(), parentPoint.Y(), parentPoint.Z(), 1 };
	vecMatrix.dotInPlace(getParentToNodeMatrix());
	return vecMatrix.getPartOfMatix<1, 3>(0, 0).getData();
}

void TransformComponent::render()
{
	Matrix4 worldMat = getNodeToParentMatrix();
	ConstantBufferManager::instance()->setVSMatrix(8, worldMat);
}
