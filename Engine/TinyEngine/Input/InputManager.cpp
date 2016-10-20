#include "TinyEngine/precomp.h"
#include "InputManager.h"
#include "TinyEngine/Engine/Engine.h"

PlatformInputManager* InputManager::instance()
{
	static PlatformInputManager ins;
	return &ins;
}

ButtonStatus InputManager::getVitualBtnStatus(VirtualButton btn)
{
	ButtonStatus status;
	auto it = _btnChangeChache.find(btn);
	if (it != _btnChangeChache.end())
	{
		status.isDown = it->second.isDown;
		status.timeStamp = it->second.timeStamp;
		status.isChangedInThisFrame = true;
	}
	else
	{
		status.isDown = _vBtnStatus[(int)btn][0].isDown;
		status.timeStamp = _vBtnStatus[(int)btn][0].timeStamp;
		status.isChangedInThisFrame = false;
	}
	return status;
}

ButtonStatus InputManager::getVitualBtnLastStatus(VirtualButton btn)
{
	ButtonStatus status;
	auto it = _btnChangeChache.find(btn);
	if (it != _btnChangeChache.end())
	{
		status.isDown = _vBtnStatus[(int)btn][0].isDown;
		status.timeStamp = _vBtnStatus[(int)btn][0].timeStamp;
		status.isChangedInThisFrame = false;
	}
	else
	{
		status.isDown = _vBtnStatus[(int)btn][1].isDown;
		status.timeStamp = _vBtnStatus[(int)btn][1].timeStamp;
		status.isChangedInThisFrame = false;
	}
	return status;
}

void InputManager::update(float dt)
{
	for (auto& pa : _btnChangeChache)
	{
		VirtualButton btn = pa.first;
		VBtnStatusSave& status = pa.second;

		_vBtnStatus[(int)btn][1] = _vBtnStatus[(int)btn][0];
		_vBtnStatus[(int)btn][0] = status;
	}
	_btnChangeChache.clear();
}

void InputManager::onVirtualBtnDown(VirtualButton btn)
{
	TinyAssert(btn < VirtualButton::VIRTUAL_BTN_NUMBER);
	_btnChangeChache[btn] = VBtnStatusSave(true,Engine::instance()->getTime());
}

void InputManager::onVirtualBtnUp(VirtualButton btn)
{
	TinyAssert(btn < VirtualButton::VIRTUAL_BTN_NUMBER);
	_btnChangeChache[btn] = VBtnStatusSave(false, Engine::instance()->getTime());
}
