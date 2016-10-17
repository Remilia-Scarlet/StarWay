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

void TransformComponent::faceToDir(const Vector3& lookAt)
{

}

void TransformComponent::faceToPoint(const Vector3& point)
{

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
		Matrix4 translation = Matrix4::identity();
		translation(3) = { _location.X(),_location.Y(),_location.Z(),1 };
		Matrix4 scaling = Matrix4::identity();
		scaling(0, 0) = _scale.X();
		scaling(1, 1) = _scale.Y();
		scaling(2, 2) = _scale.Z();

		_nodeToParentMatrix = (rotationMatrix * translation) * scaling;
		_nodeToParentMatrixDirty = false;
	}
	return _nodeToParentMatrix;
}

Vector3 TransformComponent::getUpDirection()
{
	return Vector3();
}

Vector3 TransformComponent::getFrontDirection()
{
	return Vector3();
}

void TransformComponent::render()
{
	Matrix4 worldMat = getNodeToWorldMatrix().transpose();
	ConstantBufferManager::instance()->setVSMatrix(8, worldMat);
}
