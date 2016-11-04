#pragma once
#include "VirtualButton.h"
#include "Math/vector/Vector.h"
#include "TinyEngine/ScriptManager/LuaManager.h"

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	class InputManagerWin;
	typedef InputManagerWin PlatformInputManager;
#else
	#error not support
#endif


class InputManager
{
public:
	static bool createLuaPrototype();
public:
	static bool createInstance();
	static PlatformInputManager* instance();
	static void destroyInstance();

	// return current status of a virtual btn
	virtual ButtonStatus getVitualBtnStatus(VirtualButton btn);
	LUA_SINGLETON_FUN_P1R0_DECL(InputManager,getVitualBtnStatus, VirtualButton);

	// return last change status. ButtonStatus.isDown is last down/up status, ButtonStatus.timeStamp is the time when last changing happened, ButtonStatus.isChangedInThisFrame is always false.
	virtual ButtonStatus getVitualBtnLastStatus(VirtualButton btn);
	LUA_SINGLETON_FUN_P1R0_DECL(InputManager, getVitualBtnLastStatus, VirtualButton);

	// get mouse position
	virtual Vector2 getVirtualMousePos();
	LUA_SINGLETON_FUN_P0R1_DECL(InputManager, getVirtualMousePos);

	// get mouse btn status
	virtual MouseBtnStatus getVirtualMouseStatus(VirtualMouseBtn btn);
	LUA_SINGLETON_FUN_P1R0_DECL(InputManager, getVirtualMouseStatus, VirtualMouseBtn);

	// return last click mouse status
	virtual MouseBtnStatus getVirtualMouseLastStatus(VirtualMouseBtn btn);
	LUA_SINGLETON_FUN_P1R0_DECL(InputManager, getVirtualMouseLastStatus, VirtualMouseBtn);

	virtual void update(float dt);
protected:
	virtual bool init();
	// map platform btn to virtual btn
	virtual void mapVirtualBtns() = 0;
	virtual void onVirtualBtnDown(VirtualButton btn);
	virtual void onVirtualBtnUp(VirtualButton btn);

	virtual void onVirtualMouseBtnDown(VirtualMouseBtn btn, const Vector2& pos);
	virtual void onVirtualMouseBtnUp(VirtualMouseBtn btn, const Vector2& pos);

	struct VBtnStatusSave
	{
		VBtnStatusSave() :isDown(false),timeStamp(0){}
		VBtnStatusSave(bool isDown, float timeStamp):isDown(isDown),timeStamp(timeStamp) {}
		bool isDown;
		float timeStamp;
	};
	VBtnStatusSave _vBtnStatus[(int)VirtualButton::VIRTUAL_BTN_NUMBER][2];
	std::unordered_map<VirtualButton, VBtnStatusSave> _btnChangeChache;

	struct VMouseStatusSave
	{
		VMouseStatusSave() :isDown(false), timeStamp(0) {}
		VMouseStatusSave(bool isDown, float timeStamp, const Vector2& pos) :isDown(isDown), timeStamp(timeStamp), position(pos) {}
		bool isDown;
		float timeStamp;
		Vector2 position;
	};
	VMouseStatusSave _vMouseBtnStatus[(int)VirtualMouseBtn::VIRTUAL_MOUSE_BTN_NUM][2];
	std::unordered_map<VirtualMouseBtn, VMouseStatusSave> _mouseBtnChangeChache;

	Vector2 _vMousePos;
protected:
	static PlatformInputManager* s_instance;
};

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	#include "Windows/InputManagerWin.h"
#else
	#error not support
#endif
