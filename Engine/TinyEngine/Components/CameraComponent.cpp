#include "TinyEngine/precomp.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Graphic/Manager/ConstantBufferManager.h"
#include "Math/vector/Vector.h"
#include "Math/quaternion/Quaternion.h"
#include "Math/matrix/Matrix.h"
#include <DirectXMath.h>
#include "TinyEngine/Engine/Engine.h"


CameraComponent::CameraComponent()
	:BaseComponent()
	, _fieldOfView(DEFAULT_FIELD_OF_VIEW)
	, _nearClipPlane(DEFAULT_NEAR_CLIP_PLANE)
	, _farClipPlane(DEFAULT_FAR_CLIP_PLANE)
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

void CameraComponent::setFiledOfView(float angle)
{
	_fieldOfView = angle;
}

float CameraComponent::getFiledOfView() const
{
	return _fieldOfView;
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
	GraphicMgr::instance()->setViewPort(_viewPort);

	// set view matrix
	Matrix4 viewMatrix = trans->getRotation().conjugate().toRotationMatrix();
	viewMatrix.dotInPlace(CreateTranslaionMatrixFromVector(Vector3(0, 0, 0) - trans->getLocation()));
	ConstantBufferManager::instance()->setVSMatrix(0, viewMatrix);

	// set proj matrix
	Matrix4 projMatrix = CreateProjMatrix(_fieldOfView, _viewPort.Width / _viewPort.Height, _nearClipPlane, _farClipPlane);
	ConstantBufferManager::instance()->setVSMatrix(4, projMatrix);
}

bool CameraComponent::init()
{
	return true;
}
