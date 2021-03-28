#pragma once
#include "DangoEngine/Components/ScriptComponent.h"
#include "DangoMath/vector/Vector.h"
#include "DangoEngine/Components/TransformComponent.h"

class PointLightMovement : public Script
{
public:
	const float	ROTATE_RANDIUS = 5.0;
	const float	ROTATE_SPEED = 3;
	DangoMath::Vector3	_originalPos;
	float _detaTime = 0;
public:
	virtual void init() override
	{
		ObjectPtr obj = _owner.lock();
		TransformComponentPtr transformCom = obj->getComponent<TransformComponent>();
		if (transformCom == nullptr)
			return;
		_originalPos = transformCom->getLocation();
	}
	virtual void update(float dt) override
	{
		ObjectPtr obj = _owner.lock();
		TransformComponentPtr transformCom = obj->getComponent<TransformComponent>();
		if (transformCom == nullptr)
			return;
		_detaTime = _detaTime + dt;
		if (_detaTime > ROTATE_SPEED)
		{
			_detaTime = _detaTime - ROTATE_SPEED;
		}
		float x = _originalPos.X();
		float y = _originalPos.Z() + ROTATE_RANDIUS;
		float x0 = _originalPos.X();
		float y0 = _originalPos.Z();
		float angle = _detaTime / ROTATE_SPEED * static_cast<float>(M_PI) * 2.0f;
		float cosTheta = cos(angle);
		float sinTheta = sin(angle);
		x = (x - x0) * cosTheta - (y - y0) * sinTheta + x0;
		y = (x - x0) * sinTheta + (y - y0) * cosTheta + y0;
		transformCom->setLocation(Vector3(x, _originalPos.Y(), y));
	}
};
