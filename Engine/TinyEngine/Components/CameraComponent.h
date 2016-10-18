#pragma once
#include "TinyEngine/Components/BaseComponent.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix.h"

TINY_DEFINE_PTR(CameraComponent);
class CameraComponent : public BaseComponent
{
public:
	CameraComponent();
	virtual ~CameraComponent();
public:
	static CameraComponentPtr create();
public:
	virtual void render() override;

protected:
	virtual bool init();
};

