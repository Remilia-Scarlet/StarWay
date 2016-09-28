#include "TinyEngine\precomp.h"
#include "TransformComponent.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include "TinyEngine\Engine\Engine.h"
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\GraphicMgr.h"


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
	Vector2 vec(1.2f, 1.3f);
	Vector2 vec0;
	if (vec0 == vec)
	{
		int a = 0;
	}
	vec0 = 1.2f;

	//DirectX::XMMATRIX proj;
	//proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, Engine::instance()->getSolutionWidth() / (FLOAT)Engine::instance()->getSolutionHeight(), 0.01f, 100.0f);
	//proj = XMMatrixTranspose(proj);
	Matrix4 projMat;
	ConstantBufferManager::instance()->setVSMatrix(0, projMat);

	//struct World
	//{
	//	DirectX::XMMATRIX world;
	//};
	//World w;
	//static float t = 0.0f;
	//static DWORD dwTimeStart = 0;
	//DWORD dwTimeCur = GetTickCount();
	//if (dwTimeStart == 0)
	//	dwTimeStart = dwTimeCur;
	//t = (dwTimeCur - dwTimeStart) / 1000.0f;
	//// Rotate cube around the origin
	//w.world = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(t));
	//ConstantBufferManager::instance()->setConstantBuffer(2, &w, sizeof(w), ConstantBufferManager::VS);
	Matrix4 worldMat;
	ConstantBufferManager::instance()->setVSMatrix(1, worldMat);
}
