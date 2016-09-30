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
	, _scale(1.f,1.f,1.f)
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

void TransformComponent::faceToDir(const Vector3& lookAt)
{

}

void TransformComponent::faceToPoint(const Vector3& point)
{

}

void TransformComponent::setRotation(float deltaX, float deltaY, float deltaZ)
{
	
}

void TransformComponent::setRotation(const Quaternion& rotation)
{

}

const Matrix4& TransformComponent::getWorldMatrix()
{
	if (_nodeToParentMatrixDirty)
	{
		ObjectPtr owner = _owner.lock();
		TinyAssert(owner.isValid());



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

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world = XMMatrixTranspose(world);
	Matrix4 worldMat = getWorldMatrix();
	memcpy(&worldMat, &world, sizeof(worldMat));
	ConstantBufferManager::instance()->setVSMatrix(8, worldMat);
}
