#pragma once
#include "TinyEngine/Components/BaseComponent.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix.h"
#include "Graphic/Manager/GraphicMgr.h"

TINY_DEFINE_PTR(CameraComponent);
class CameraComponent : public BaseComponent
{
public:
	const float DEFAULT_FIELD_OF_VIEW = 60.f;
	const float DEFAULT_NEAR_CLIP_PLANE = 0.02f;
	const float DEFAULT_FAR_CLIP_PLANE = 400.f;
public:
	virtual ~CameraComponent();
public:
	static CameraComponentPtr create();
public:
	// set field of view, in degree. Default is DEFAULT_FIELD_OF_VIEW
	void setFiledOfView(float angle);

	// get filed of view in degree.
	float getFiledOfView() const;
	// 
public:
	virtual void render() override;

protected:
	CameraComponent();
	virtual bool init();

	float _fieldOfView;
	ViewPort _viewPort;
	float _farClipPlane;
	float _nearClipPlane;
};

