#pragma once
#include "DangoEngine/Components/ScriptComponent.h"
#include "DangoEngine/Components/TransformComponent.h"
#include "DangoEngine/Input/InputManager.h"

class CameraControl : public Script
{
public:
	const float MOVE_SPEED = 0.1f;
	const float TURN_SPEED = 0.1f;
	bool _lastLIsDown = false;
	Vector2 _lastMousePos = {0, 0};
public:
	void update(float dt) override
	{
		handleMouse(dt);
		handleKey();
	}

	void handleMouse(float dt)
	{
		if (!_owner.isValid())
			return;

		ObjectPtr obj = _owner.lock();

		TransformComponentPtr transformCom = obj->getComponent<TransformComponent>();
		if (transformCom == nullptr)
		{
			return;
		}

		MouseBtnStatus mouseRStatus = InputManager::instance()->getVirtualMouseStatus(VirtualMouseBtn::R);
		MouseBtnStatus mouseLStatus = InputManager::instance()->getVirtualMouseStatus(VirtualMouseBtn::L);
		if (mouseLStatus.isDown)
		{
			if (_lastLIsDown)
			{
				Vector2 currentMousePos = InputManager::instance()->getVirtualMousePos();
				float detaX = currentMousePos.X() - _lastMousePos.X();
				float detaY = currentMousePos.Y() - _lastMousePos.Y();
				Vector3 euler = transformCom->getEulerRotation();
				euler.Y() = euler.Y() + detaX * TURN_SPEED;
				euler.X() = euler.X() + detaY * TURN_SPEED;
				transformCom->setEulerRotation(euler);
				_lastMousePos = currentMousePos;
			}
			else
			{
				_lastLIsDown = true;
				_lastMousePos = InputManager::instance()->getVirtualMousePos();
			}
		}
		else
		{
			_lastLIsDown = false;
		}
	}

	void handleKey()
	{
		ObjectPtr obj = _owner.lock();
		if (!obj.isValid())
		{
			return;
		}
		TransformComponentPtr transformCom = obj->getComponent<TransformComponent>();
		if (!transformCom.isValid())
			return;

		ButtonStatus btnUpStatus = InputManager::instance()->getVitualBtnStatus(VirtualButton::UP);
		ButtonStatus btnDownStatus = InputManager::instance()->getVitualBtnStatus(VirtualButton::DOWN);
		ButtonStatus btnLeftStatus = InputManager::instance()->getVitualBtnStatus(VirtualButton::LEFT);
		ButtonStatus btnRightStatus = InputManager::instance()->getVitualBtnStatus(VirtualButton::RIGHT);
		ButtonStatus btnL1Status = InputManager::instance()->getVitualBtnStatus(VirtualButton::L1);
		ButtonStatus btnR1Status = InputManager::instance()->getVitualBtnStatus(VirtualButton::R1);
		ButtonStatus btnPauseStatus = InputManager::instance()->getVitualBtnStatus(VirtualButton::Pause);
		if (!btnPauseStatus.isDown && !btnUpStatus.isDown && !btnDownStatus.isDown && !btnRightStatus.isDown
			&& !btnLeftStatus.isDown && !btnL1Status.isDown && !btnR1Status.isDown)
			return;

		Vector3 location = transformCom->getLocation();
		Vector3 frontDirection = transformCom->getFrontDirection();
		Vector3 rightDirection = transformCom->getRightDirection();
		if (btnUpStatus.isDown)
		{
			location.X() = location.X() + frontDirection.X() * MOVE_SPEED;
			location.Y() = location.Y() + frontDirection.Y() * MOVE_SPEED;
			location.Z() = location.Z() + frontDirection.Z() * MOVE_SPEED;
		}
		else if (btnDownStatus.isDown)
		{
			location.X() = location.X() - frontDirection.X() * MOVE_SPEED;
			location.Y() = location.Y() - frontDirection.Y() * MOVE_SPEED;
			location.Z() = location.Z() - frontDirection.Z() * MOVE_SPEED;
		}

		if (btnRightStatus.isDown)
		{
			location.X() = location.X() + rightDirection.X() * MOVE_SPEED;
			location.Y() = location.Y() + rightDirection.Y() * MOVE_SPEED;
			location.Z() = location.Z() + rightDirection.Z() * MOVE_SPEED;
		}
		else if (btnLeftStatus.isDown)
		{
			location.X() = location.X() - rightDirection.X() * MOVE_SPEED;
			location.Y() = location.Y() - rightDirection.Y() * MOVE_SPEED;
			location.Z() = location.Z() - rightDirection.Z() * MOVE_SPEED;
		}

		if (btnL1Status.isDown)
		{
			Vector3 parentUp = transformCom->parentVectorToLocal(Vector3(0, 1, 0));
			location.X() = location.X() + parentUp.X() * MOVE_SPEED;
			location.Y() = location.Y() + parentUp.Y() * MOVE_SPEED;
			location.Z() = location.Z() + parentUp.Z() * MOVE_SPEED;
		}
		else if (btnR1Status.isDown)
		{
			Vector3 parentUp = transformCom->parentVectorToLocal(Vector3(0, 1, 0));
			location.X() = location.X() - parentUp.X() * MOVE_SPEED;
			location.Y() = location.Y() - parentUp.Y() * MOVE_SPEED;
			location.Z() = location.Z() - parentUp.Z() * MOVE_SPEED;
		}

		if (btnPauseStatus.isDown)
		{
			transformCom->setLocation(Vector3(0, 2, -5));
		}
		else
		{
			transformCom->setLocation(location);
		}
	}
};
