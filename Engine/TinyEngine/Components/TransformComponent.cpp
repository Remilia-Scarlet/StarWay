#include "TinyEngine\precomp.h"
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
	:BaseComponent()
	, _scale{1.f,1.f,1.f}
	, _nodeToParentMatrixDirty(true)
	, _nodeToParentMatrix(Matrix4::identity())
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
	_nodeToParentMatrixDirty = true;
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

void TransformComponent::setRotation(float deltaX, float deltaY, float deltaZ)
{
	setRotation(Quaternion(deltaX, deltaY, deltaZ));
}

void TransformComponent::setRotation(const Quaternion& rotation)
{
	_rotate = rotation;
	_nodeToParentMatrixDirty = true;
}

const Quaternion& TransformComponent::getRotation()
{
	return _rotate;
}

void TransformComponent::setScale(float x, float y, float z)
{
	_scale.X() = x;
	_scale.Y() = y;
	_scale.Z() = z;
	_nodeToParentMatrixDirty = true;
}

void TransformComponent::setScale(const Vector3& scale)
{
	setScale(scale.X(), scale.Y(), scale.Z());
}

const Vector3& TransformComponent::getScale()
{
	return _scale;
}

const Matrix4& TransformComponent::getNodeToWorldMatrix()
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

Vector3 TransformComponent::getUpDirection()
{
	return getNodeToWorldMatrix().getRow(1).subVecter<0,3>();
}

Vector3 TransformComponent::getFrontDirection()
{
	return getNodeToWorldMatrix().getRow(2).subVecter<0, 3>();
}

void TransformComponent::render()
{
	Matrix4 worldMat = getNodeToWorldMatrix();
	ConstantBufferManager::instance()->setVSMatrix(8, worldMat);
}
