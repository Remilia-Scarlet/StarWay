#include "TinyEngine\precomp.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include "Math/vector/Vector.h"
#include "Math/quaternion/Quaternion.h"
#include "Math/matrix/Matrix4.h"


CameraComponent::CameraComponent()
	:BaseComponent()
{
}


CameraComponent::~CameraComponent()
{
}

CameraComponentPtr CameraComponent::create()
{
	CameraComponent* ret = new CameraComponent();
	if (!ret || !ret->init())
		TINY_SAFE_DELETE(ret);
	return CameraComponentPtr(ret);
}

void CameraComponent::lookAtDir(const Vector3& lookAt)
{
	throw "not support yet";
}

void CameraComponent::lookAtPoint(const Vector3& point)
{
	//TinyAssert(!_owner.expired(),"need owner");
	//if (_owner.expired())
	//	return;

	//ObjectPtr owner = _owner.lock();
	//TransformComponentPtr trans = owner->getComponent<TransformComponentPtr>();
	//if (!trans)
	//	return;

	//Matrix4 viewMatrix = getLookAtPointMatrix(trans->getLocation(), trans->getUpDirection(), point);
	//Vector3 rotation(viewMatrix(0, 0), viewMatrix(1, 1), viewMatrix(2, 2));
	//trans->setRotation(rotation);
}

void CameraComponent::render()
{
	//set view port
	//if (_owner.expired())
	//	return;

	//ObjectPtr owner = _owner.lock();
	//TransformComponentPtr trans = owner->getComponent<TransformComponentPtr>();
	//if (!trans)
	//	return;

	//Matrix4 viewMatrix = getLookAtPointMatrix(trans->getLocation(), trans->getUpDirection(), trans->getFrontDirection());
	//viewMatrix.transposeInPlace();
	//GpuBufferManager::instance()->setConstantBuffer(0, &viewMatrix, sizeof(viewMatrix), GpuBufferManager::VS);
}

Matrix4 CameraComponent::getLookToDirectionMatrix(const Vector3& location, const Vector3& upDirection, const Vector3& eyeDirection)
{
#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	//DirectX::XMVECTOR Eye = DirectX::XMVectorSet(location(0), location(1), location(2), 0);
	//DirectX::XMVECTOR At = DirectX::XMVectorSet(eyeDirection(0), eyeDirection(1), eyeDirection(2), 0);
	//DirectX::XMVECTOR Up = DirectX::XMVectorSet(upDirection(0), upDirection(1), upDirection(2), 0);
	//DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(Eye, At, Up);
	//Matrix4 ret((float*)&view);
	//return ret;
	throw "not support";
#else
#error unsupport fucntion
#endif
}

Matrix4 CameraComponent::getLookAtPointMatrix(const Vector3& location, const Vector3& upDirection, const Vector3& lookAtPoint)
{
#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	//DirectX::XMVECTOR Eye = DirectX::XMVectorSet(location(0), location(1), location(2), 0);
	//DirectX::XMVECTOR At = DirectX::XMVectorSet(lookAtPoint(0), lookAtPoint(1), lookAtPoint(2), 0);
	//DirectX::XMVECTOR Up = DirectX::XMVectorSet(upDirection(0), upDirection(1), upDirection(2), 0);
	//DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(Eye, At, Up);
	//Matrix4 ret((float*)&view);
	//return ret;
	return Matrix4();
#else
#error unsupport fucntion
#endif

}

bool CameraComponent::init()
{
	return true;
}
