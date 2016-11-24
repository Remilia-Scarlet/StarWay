#include "TinyEngine/precomp.h"
#include "InputManager.h"
#include "TinyEngine/Engine/Engine.h"

PlatformInputManager* InputManager::s_instance = nullptr;

bool InputManager::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(getVitualBtnStatus);
	LUA_PROTOTYPE_REGIST_FUN(getVitualBtnLastStatus);
	LUA_PROTOTYPE_REGIST_FUN(getVirtualMouseStatus);
	LUA_PROTOTYPE_REGIST_FUN(getVirtualMouseLastStatus);
	LUA_PROTOTYPE_REGIST_FUN(getVirtualMousePos);

#define REGIST_VIRTUAL_BTN(NAME) LUA_PROTOTYPE_REGIST_CONST_VAL("BTN_"###NAME, (int)VirtualButton::##NAME);
	REGIST_VIRTUAL_BTN(A);
	REGIST_VIRTUAL_BTN(B);
	REGIST_VIRTUAL_BTN(X);
	REGIST_VIRTUAL_BTN(Y); 
	REGIST_VIRTUAL_BTN(L1);
	REGIST_VIRTUAL_BTN(L2);
	REGIST_VIRTUAL_BTN(L3);
	REGIST_VIRTUAL_BTN(R1);
	REGIST_VIRTUAL_BTN(R2);
	REGIST_VIRTUAL_BTN(R3);
	REGIST_VIRTUAL_BTN(Pause);
	REGIST_VIRTUAL_BTN(Start);
	REGIST_VIRTUAL_BTN(UP);
	REGIST_VIRTUAL_BTN(LEFT);
	REGIST_VIRTUAL_BTN(RIGHT);
	REGIST_VIRTUAL_BTN(DOWN);
#undef REGIST_VIRTUAL_BTN

#define REGIST_VIRTUAL_MOUSE(NAME) LUA_PROTOTYPE_REGIST_CONST_VAL("MOUSE_"###NAME, (int)VirtualMouseBtn::##NAME);
	REGIST_VIRTUAL_MOUSE(L);
	REGIST_VIRTUAL_MOUSE(R);
#undef REGIST_VIRTUAL_MOUSE

	LUA_PROTOTYPE_END(InputManager);
	return true;
}

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

int InputManager::L_getVitualBtnStatus(lua_State* L)
{
	int btn = LuaManager::instance()->getVal<int>(L, 2);
	if (btn < 0 || btn >= (int)VirtualButton::VIRTUAL_BTN_NUMBER )
		return LUA_PARAM_ERROR(InputManager::L_getVitualBtnStatus);
	ButtonStatus status = InputManager::instance()->getVitualBtnStatus((VirtualButton)btn);
	LuaVal val = {_K("isDown") = status.isDown, _K("timeStamp") = status.timeStamp, _K("isChangedInThisFrame") = status.isChangedInThisFrame};
	LuaManager::instance()->pushVal(L, val);
	return 1;
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

int InputManager::L_getVitualBtnLastStatus(lua_State* L)
{
	int btn = LuaManager::instance()->getVal<int>(L, 2);
	if (btn < 0 || btn >= (int)VirtualButton::VIRTUAL_BTN_NUMBER)
		return LUA_PARAM_ERROR(InputManager::L_getVitualBtnLastStatus);
	ButtonStatus status = InputManager::instance()->getVitualBtnLastStatus((VirtualButton)btn);
	LuaVal val = { _K("isDown") = status.isDown, _K("timeStamp") = status.timeStamp, _K("isChangedInThisFrame") = status.isChangedInThisFrame };
	LuaManager::instance()->pushVal(L, val);
	return 1;
}

Vector2 InputManager::getVirtualMousePos()
{
	return _vMousePos;
}

LUA_SINGLETON_FUN_P0R1_IMPL(InputManager, getVirtualMousePos);

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

int InputManager::L_getVirtualMouseStatus(lua_State* L)
{
	int btn = LuaManager::instance()->getVal<int>(L, 2);
	if (btn < 0 || btn >= (int)VirtualMouseBtn::VIRTUAL_MOUSE_BTN_NUM)
		return LUA_PARAM_ERROR(InputManager::L_getVirtualMouseStatus);
	MouseBtnStatus status = InputManager::instance()->getVirtualMouseStatus((VirtualMouseBtn)btn);
	LuaVal val = {
		_K("isDown") = status.isDown,
		_K("timeStamp") = status.timeStamp,
		_K("isChangedInThisFrame") = status.isChangedInThisFrame ,
		_K("pos") = {_K("x") = status.position.X(),_K("y") = status.position.Y()}
	};
	LuaManager::instance()->pushVal(L, val);
	return 1;
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

int InputManager::L_getVirtualMouseLastStatus(lua_State* L)
{
	int btn = LuaManager::instance()->getVal<int>(L, 2);
	if (btn < 0 || btn >= (int)VirtualMouseBtn::VIRTUAL_MOUSE_BTN_NUM)
		return LUA_PARAM_ERROR(InputManager::getVirtualMouseLastStatus);
	MouseBtnStatus status = InputManager::instance()->getVirtualMouseLastStatus((VirtualMouseBtn)btn);
	LuaVal val = {
		_K("isDown") = status.isDown,
		_K("timeStamp") = status.timeStamp,
		_K("isChangedInThisFrame") = status.isChangedInThisFrame ,
		_K("pos") = { _K("x") = status.position.X(),_K("y") = status.position.Y() }
	};
	LuaManager::instance()->pushVal(L, val);
	return 1;
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
