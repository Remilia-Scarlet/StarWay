#pragma once
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
	VIRTUAL_BTN_NUMBER
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
