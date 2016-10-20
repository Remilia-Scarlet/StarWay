#pragma once
#include "VirtualButton.h"

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	class InputManagerWin;
	typedef InputManagerWin PlatformInputManager;
#else
	#error not support
#endif


class InputManager
{
public:
	static PlatformInputManager* instance();

	// return status of a virtual btn
	virtual ButtonStatus getVitualBtnStatus(VirtualButton btn);

	// return last change status. ButtonStatus.isDown is last down/up status. ButtonStatus.timeStamp is the time when last changing happened.
	virtual ButtonStatus getVitualBtnLastStatus(VirtualButton btn);

	virtual void update(float dt);
protected:
	// map platform btn to virtual btn
	virtual void mapVirtualBtns() = 0;
	virtual void onVirtualBtnDown(VirtualButton btn);
	virtual void onVirtualBtnUp(VirtualButton btn);

	struct VBtnStatusSave
	{
		VBtnStatusSave() :isDown(false),timeStamp(0){}
		VBtnStatusSave(bool isDown, float timeStamp):isDown(isDown),timeStamp(timeStamp) {}
		bool isDown;
		float timeStamp;
	};
	VBtnStatusSave _vBtnStatus[(int)VirtualButton::VIRTUAL_BTN_NUMBER][2];
	std::unordered_map<VirtualButton, VBtnStatusSave> _btnChangeChache;
};

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	#include "Windows/InputManagerWin.h"
#else
	#error not support
#endif
