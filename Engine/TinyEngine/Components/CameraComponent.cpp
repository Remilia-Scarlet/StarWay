#include "TinyEngine\precomp.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include "Math/vector/Vector.h"
#include "Math/quaternion/Quaternion.h"
#include "Math/matrix/Matrix.h"
#include <DirectXMath.h>
#include "TinyEngine\Engine\Engine.h"


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

	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 2.0f, -5.0f, 0.0f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 11.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, At, Up);
	Matrix4 viewMatrix;
	memcpy(&viewMatrix,&View,sizeof(View));
	ConstantBufferManager::instance()->setVSMatrix(0, viewMatrix);

	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, Engine::instance()->getSolutionWidth() / (FLOAT)Engine::instance()->getSolutionHeight(), 0.01f, 100.0f);
	Matrix4 projMatrix;
	memcpy(&projMatrix, &Projection, sizeof(Projection));
	ConstantBufferManager::instance()->setVSMatrix(4, projMatrix);
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
