#include "precomp.h"
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

bool CameraComponent::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);
	LUA_PROTOTYPE_REGIST_FUN(setFiledOfView);
	LUA_PROTOTYPE_REGIST_FUN(getFiledOfView);
	LUA_PROTOTYPE_REGIST_FUN(setFarClipPlane);
	LUA_PROTOTYPE_REGIST_FUN(getFarClipPlane);
	LUA_PROTOTYPE_REGIST_FUN(setNearClipPlane);
	LUA_PROTOTYPE_REGIST_FUN(getNearClipPlane);
	LUA_PROTOTYPE_REGIST_FUN(setDrawRectOnScreen);
	LUA_PROTOTYPE_REGIST_FUN(getRawRectOnScreen);

	LUA_PROTOTYPE_END(CameraComponent);
	return true;
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

LUA_MEMBER_FUN_P1R0_IMPL(CameraComponent, setFiledOfView, float);

float CameraComponent::getFiledOfView() const
{
	return _fieldOfView;
}

LUA_MEMBER_FUN_P0R1_IMPL(CameraComponent, getFiledOfView);

void CameraComponent::setFarClipPlane(float farZ)
{
	_farClipPlane = farZ;
}

LUA_MEMBER_FUN_P1R0_IMPL(CameraComponent, setFarClipPlane, float);

float CameraComponent::getFarClipPlane() const
{
	return _farClipPlane;
}

LUA_MEMBER_FUN_P0R1_IMPL(CameraComponent, getFarClipPlane);

void CameraComponent::setNearClipPlane(float nearZ)
{
	_nearClipPlane = nearZ;
}

LUA_MEMBER_FUN_P1R0_IMPL(CameraComponent, setNearClipPlane, float);

float CameraComponent::getNearClipPlane() const
{
	return _nearClipPlane;
}

LUA_MEMBER_FUN_P0R1_IMPL(CameraComponent, getNearClipPlane);

void CameraComponent::setDrawRectOnScreen(const Rect& rect)
{
	_viewPort.TopLeftX = rect.X();
	_viewPort.TopLeftY = rect.Y();
	_viewPort.Width = rect.W();
	_viewPort.Height = rect.H();
}

LUA_MEMBER_FUN_P1R0_IMPL(CameraComponent, setDrawRectOnScreen, const Rect&);

Rect CameraComponent::getRawRectOnScreen() const
{
	return Rect(_viewPort.TopLeftX, _viewPort.TopLeftY, _viewPort.Width, _viewPort.Height);
}

LUA_MEMBER_FUN_P0R1_IMPL(CameraComponent, getRawRectOnScreen);

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
	ConstantBufferManager::instance()->setVSMatrix(0, viewMatrix);

	// set proj matrix
	Matrix4 projMatrix = CreatePerspectiveProjMatrix(_fieldOfView, _viewPort.Width / _viewPort.Height, _nearClipPlane, _farClipPlane);
	ConstantBufferManager::instance()->setVSMatrix(4, projMatrix);

	// set camera pos
	ConstantBufferManager::instance()->setPSVector(0, Vector4(trans->getLocation().X(), trans->getLocation().Y(), trans->getLocation().Z(), 0));
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
