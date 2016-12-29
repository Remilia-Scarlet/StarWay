#pragma once
#include "TinyEngine/Components/BaseComponent.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix.h"
#include "Graphic/Manager/GraphicMgr.h"
#include "Math/shape/Rect.h"
#include "TinyEngine/ScriptManager/LuaManager.h"

TINY_DEFINE_PTR(CameraComponent);
class CameraComponent : public BaseComponent
{
public:
	const float DEFAULT_FIELD_OF_VIEW = 60.f;
	const float DEFAULT_NEAR_CLIP_PLANE = 0.02f;
	const float DEFAULT_FAR_CLIP_PLANE = 400.f;
public:
	static bool createLuaPrototype();
public:
	static CameraComponentPtr create();
	LUA_CREATE_FUN_P0(CameraComponent);

public:
	// set field of view, in degree. Default is DEFAULT_FIELD_OF_VIEW
	void setFiledOfView(float angle);
	LUA_MEMBER_FUN_P1R0_DECL(CameraComponent, setFiledOfView, float);

	// get filed of view in degree.
	float getFiledOfView() const;
	LUA_MEMBER_FUN_P0R1_DECL(CameraComponent, getFiledOfView);

	// set far clip plane
	void setFarClipPlane(float farZ);
	LUA_MEMBER_FUN_P1R0_DECL(CameraComponent, setFarClipPlane, float);

	// get far clip plane
	float getFarClipPlane() const;
	LUA_MEMBER_FUN_P0R1_DECL(CameraComponent, getFarClipPlane);

	// set near clip plane
	void setNearClipPlane(float nearZ);
	LUA_MEMBER_FUN_P1R0_DECL(CameraComponent, setNearClipPlane, float);

	// get near clip plane
	float getNearClipPlane() const;
	LUA_MEMBER_FUN_P0R1_DECL(CameraComponent, getNearClipPlane);

	// set draw rect in screen
	void setDrawRectOnScreen(const Rect& rect);
	LUA_MEMBER_FUN_P1R0_DECL(CameraComponent, setDrawRectOnScreen, const Rect&);

	// get draw rect in screen
	Rect getRawRectOnScreen() const;
	LUA_MEMBER_FUN_P0R1_DECL(CameraComponent, getRawRectOnScreen);

public:
	virtual void render() override;
	virtual void setOwner(const RefCountPtr<Object> & owner) override;
protected:
	CameraComponent();
	~CameraComponent();
	virtual bool init();

	float _fieldOfView;
	ViewPort _viewPort;
	float _farClipPlane;
	float _nearClipPlane;
};

