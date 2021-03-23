#pragma once
#include "Math\vector\Vector.h"
enum class VirtualButton
{
	A,
	B,
	X,
	Y,
	L1,
	L2,
	L3,
	R1,
	R2,
	R3,
	Pause,
	Start,
	UP,
	LEFT,
	RIGHT,
	DOWN,
	VIRTUAL_BTN_NUMBER
};
enum class VirtualMouseBtn
{
	L,
	R,
	VIRTUAL_MOUSE_BTN_NUM
};

struct ButtonStatus
{
	// is the button down
	bool isDown;

	// the time stamp when btn status changed
	float timeStamp;

	// is button down or up at this frame
	bool isChangedInThisFrame;

	// isDown && isChangedInThisFrame
	inline bool isThisFrameDown()
	{
		return isDown && isChangedInThisFrame;
	}

	// !isDown && isChangedInThisFrame
	inline bool isThisFrameUp()
	{
		return !isDown && isChangedInThisFrame;
	}
};

struct MouseBtnStatus
{
	// is the button down
	bool isDown;

	// the time stamp when btn status changed
	float timeStamp;

	// is button down or up at this frame
	bool isChangedInThisFrame;

	// when status change, the position of mouse
	Vector2 position;

	// isDown && isChangedInThisFrame
	inline bool isThisFrameDown()
	{
		return isDown && isChangedInThisFrame;
	}

	// !isDown && isChangedInThisFrame
	inline bool isThisFrameUp()
	{
		return !isDown && isChangedInThisFrame;
	}
};