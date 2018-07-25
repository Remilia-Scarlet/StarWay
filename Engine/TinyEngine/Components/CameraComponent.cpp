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
	:BaseComponent(TO_STRING(CameraComponent))
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

void CameraComponent::setFarClipPlane(float farZ)
{
	_farClipPlane = farZ;
}

float CameraComponent::getFarClipPlane() const
{
	return _farClipPlane;
}

void CameraComponent::setNearClipPlane(float nearZ)
{
	_nearClipPlane = nearZ;
}

float CameraComponent::getNearClipPlane() const
{
	return _nearClipPlane;
}

void CameraComponent::setDrawRectOnScreen(const Rect& rect)
{
	_viewPort.TopLeftX = rect.X();
	_viewPort.TopLeftY = rect.Y();
	_viewPort.Width = rect.W();
	_viewPort.Height = rect.H();
}

Rect CameraComponent::getRawRectOnScreen() const
{
	return Rect(_viewPort.TopLeftX, _viewPort.TopLeftY, _viewPort.Width, _viewPort.Height);
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
	Matrix4 viewMatrix = trans->getParentToNodeMatrix();
	ConstantBufferManager::instance()->getVsGlobalConstant().setViewMatrix(viewMatrix);

	// set proj matrix
	Matrix4 projMatrix = CreatePerspectiveProjMatrix(_fieldOfView, _viewPort.Width / _viewPort.Height, _nearClipPlane, _farClipPlane);
	ConstantBufferManager::instance()->getVsGlobalConstant().setProjectionMatrix(projMatrix);

	// set camera pos
	ConstantBufferManager::instance()->getPsGlobalConstant().setCameraPos(Vector3(trans->getLocation().X(), trans->getLocation().Y(), trans->getLocation().Z()));
}

void CameraComponent::setOwner(const RefCountPtr<Object> & owner)
{
	BaseComponent::setOwner(owner);
	if (owner.isValid())
	{
		owner->setFlag(ObjectFlag::IS_CAMERA, true);
	}
}

bool CameraComponent::init()
{
	do 
	{
		_viewPort = { 0,0,(float)Engine::instance()->getSolutionWidth(),(float)Engine::instance()->getSolutionHeight(),0.f,1.f };
		return true;
	} while (0);
	return false;
}
