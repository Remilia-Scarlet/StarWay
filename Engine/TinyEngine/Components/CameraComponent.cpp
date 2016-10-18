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
	if (_owner.isValid() == false)
		return;
	ObjectPtr owner = _owner.lock();
	TransformComponentPtr trans = owner->getComponent<TransformComponent>();
	if (trans.isValid() == false)
		return;

	// set view port

	// set view matrix
	const Vector3& translation = trans->getLocation();
	Matrix4 rotation = trans->getRotation().toRotationMatrix();
	rotation(3) = { translation.X(),translation.Y(),translation.Z(),1 };


	// set proj matrix




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

bool CameraComponent::init()
{
	return true;
}
