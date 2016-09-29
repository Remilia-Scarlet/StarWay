#pragma once
#include "TinyEngine/Components/BaseComponent.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix.h"

TINY_DEFINE_PTR(CameraComponent);
class CameraComponent : public BaseComponent
{
public:
	explicit CameraComponent();
	virtual ~CameraComponent();
public:
	static CameraComponentPtr create();
public:
	virtual void render() override;

	virtual Matrix4 getLookToDirectionMatrix(const Vector3& location,const Vector3& upDirection,const Vector3& eyeDirection);
	virtual Matrix4 getLookAtPointMatrix(const Vector3& location, const Vector3& upDirection, const Vector3& lookAtPoint);
protected:
	virtual bool init();
};

