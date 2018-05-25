#include "precomp.h"
#include "InputManager.h"
#include "TinyEngine/Engine/Engine.h"

PlatformInputManager* InputManager::s_instance = nullptr;

bool InputManager::createInstance()
{
	TinyAssert(s_instance == nullptr);
	s_instance = new PlatformInputManager();
	if (s_instance && s_instance->init())
		return true;
	TINY_SAFE_DELETE(s_instance);
	return false;
}

PlatformInputManager* InputManager::instance()
{
	return s_instance;
}

void InputManager::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
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

Vector2 InputManager::getVirtualMousePos()
{
	return _vMousePos;
}

MouseBtnStatus InputManager::getVirtualMouseStatus(VirtualMouseBtn btn)
{
	MouseBtnStatus status;
	auto it = _mouseBtnChangeChache.find(btn);
	if (it != _mouseBtnChangeChache.end())
	{
		status.isDown = it->second.isDown;
		status.timeStamp = it->second.timeStamp;
		status.isChangedInThisFrame = true;
		status.position = it->second.position;
	}
	else
	{
		status.isDown = _vMouseBtnStatus[(int)btn][0].isDown;
		status.timeStamp = _vMouseBtnStatus[(int)btn][0].timeStamp;
		status.isChangedInThisFrame = false;
		status.position = _vMouseBtnStatus[(int)btn][0].position;
	}
	return status;
}

MouseBtnStatus InputManager::getVirtualMouseLastStatus(VirtualMouseBtn btn)
{
	MouseBtnStatus status;
	auto it = _mouseBtnChangeChache.find(btn);
	if (it != _mouseBtnChangeChache.end())
	{
		status.isDown = _vMouseBtnStatus[(int)btn][0].isDown;
		status.timeStamp = _vMouseBtnStatus[(int)btn][0].timeStamp;
		status.isChangedInThisFrame = false;
		status.position = _vMouseBtnStatus[(int)btn][0].position;
	}
	else
	{
		status.isDown = _vMouseBtnStatus[(int)btn][1].isDown;
		status.timeStamp = _vMouseBtnStatus[(int)btn][1].timeStamp;
		status.isChangedInThisFrame = false;
		status.position = _vMouseBtnStatus[(int)btn][1].position;
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

	for (auto& pa : _mouseBtnChangeChache)
	{
		VirtualMouseBtn btn = pa.first;
		VMouseStatusSave& status = pa.second;

		_vMouseBtnStatus[(int)btn][1] = _vMouseBtnStatus[(int)btn][0];
		_vMouseBtnStatus[(int)btn][0] = status;
	}
	_mouseBtnChangeChache.clear();
}

bool InputManager::init()
{
	do 
	{
		mapVirtualBtns();
		return true;
	} while (0);
	return false;
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

void InputManager::onVirtualMouseBtnDown(VirtualMouseBtn btn, const Vector2& pos)
{
	TinyAssert(btn < VirtualMouseBtn::VIRTUAL_MOUSE_BTN_NUM);
	_mouseBtnChangeChache[btn] = VMouseStatusSave(true, Engine::instance()->getTime(), pos);
}

void InputManager::onVirtualMouseBtnUp(VirtualMouseBtn btn, const Vector2& pos)
{
	TinyAssert(btn < VirtualMouseBtn::VIRTUAL_MOUSE_BTN_NUM);
	_mouseBtnChangeChache[btn] = VMouseStatusSave(false, Engine::instance()->getTime(), pos);
}
