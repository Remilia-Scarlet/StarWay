#include "precomp.h"
#include "InputManagerWin.h"
#include <windows.h>


void InputManagerWin::onLocalKeyDown(int key)
{
	auto it = _VkMAp.find((LocalKey)key);
	if (it != _VkMAp.end())
		onVirtualBtnDown(it->second);
}

void InputManagerWin::onLocalKeyUp(int key)
{
	auto it = _VkMAp.find((LocalKey)key);
	if (it != _VkMAp.end())
		onVirtualBtnUp(it->second);
}

void InputManagerWin::onLocalMouseDown(int btn, int x, int y)
{
	onVirtualMouseBtnDown((btn == 1 ? VirtualMouseBtn::L : VirtualMouseBtn::R), Vector2((float)x, (float)y));
}

void InputManagerWin::onLocalMouseUp(int btn, int x, int y)
{
	onVirtualMouseBtnUp((btn == 1 ? VirtualMouseBtn::L : VirtualMouseBtn::R), Vector2((float)x, (float)y));
}

void InputManagerWin::update(float dt)
{
	POINT po;
	GetCursorPos(&po);
	_vMousePos.X() = (float)po.x;
	_vMousePos.Y() = (float)po.y;
	InputManager::update(dt);
}

void InputManagerWin::mapVirtualBtns()
{
	_VkMAp = {
		{LocalKey::KEY_W, VirtualButton::UP},
		{LocalKey::KEY_A, VirtualButton::LEFT},
		{LocalKey::KEY_D, VirtualButton::RIGHT},
		{LocalKey::KEY_S, VirtualButton::DOWN},
		{LocalKey::KEY_Q, VirtualButton::L1},
		{LocalKey::KEY_E, VirtualButton::R1},
		{LocalKey::KEY_C, VirtualButton::R3},
		{LocalKey::KEY_SPACE,VirtualButton::Pause}
	};
}
