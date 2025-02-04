#pragma once
#include "..\InputManager.h"
enum class LocalKey
{
	// Keyboard keys (not exhaustive ;-))
	KEY_BACKSPACE = 0x0008, // Same as Unicode & Win32
	KEY_RETURN = 0x000D, // Same as Unicode & Win32
	KEY_SHIFT = 0x0010, // Same as Unicode & Win32
	KEY_CTRL = 0x0011, // Same as Unicode & Win32
	KEY_ESCAPE = 0x001B, // Same as Unicode & Win32
	KEY_SPACE = 0x0020, // Same as Unicode & Win32
	KEY_PRIOR = 0x0021, // Same as Win32
	KEY_NEXT = 0x0022, // Same as Win32
	KEY_END = 0x0023, // Same as Win32
	KEY_HOME = 0x0024, // Same as Win32
	KEY_UP = 0x0026, // Same as Win32
	KEY_DOWN = 0x0028, // Same as Win32
	KEY_LEFT = 0x0025, // Same as Win32
	KEY_RIGHT = 0x0027, // Same as Win32
	KEY_DELETE = 0x002E, // Same as Unicode & Win32

	KEY_0 = 0x0030, // Same as ASCII
	KEY_1 = 0x0031, // Same as ASCII
	KEY_2 = 0x0032, // Same as ASCII
	KEY_3 = 0x0033, // Same as ASCII
	KEY_4 = 0x0034, // Same as ASCII
	KEY_5 = 0x0035, // Same as ASCII
	KEY_6 = 0x0036, // Same as ASCII
	KEY_7 = 0x0037, // Same as ASCII
	KEY_8 = 0x0038, // Same as ASCII
	KEY_9 = 0x0039, // Same as ASCII

	KEY_DOT = 0x00BE,
	KEY_MINUS = 0x00BD,

	KEY_A = 0x0041, // Same as ASCII  
	KEY_B = 0x0042, // Same as ASCII
	KEY_C = 0x0043, // Same as ASCII
	KEY_D = 0x0044, // Same as ASCII
	KEY_E = 0x0045, // Same as ASCII
	KEY_F = 0x0046, // Same as ASCII
	KEY_G = 0x0047, // Same as ASCII
	KEY_H = 0x0048, // Same as ASCII
	KEY_I = 0x0049, // Same as ASCII
	KEY_J = 0x004A, // Same as ASCII
	KEY_K = 0x004B, // Same as ASCII
	KEY_L = 0x004C, // Same as ASCII
	KEY_M = 0x004D, // Same as ASCII
	KEY_N = 0x004E, // Same as ASCII
	KEY_O = 0x004F, // Same as ASCII
	KEY_P = 0x0050, // Same as ASCII
	KEY_Q = 0x0051, // Same as ASCII
	KEY_R = 0x0052, // Same as ASCII
	KEY_S = 0x0053, // Same as ASCII
	KEY_T = 0x0054, // Same as ASCII
	KEY_U = 0x0055, // Same as ASCII
	KEY_V = 0x0056, // Same as ASCII
	KEY_W = 0x0057, // Same as ASCII
	KEY_X = 0x0058, // Same as ASCII
	KEY_Y = 0x0059, // Same as ASCII
	KEY_Z = 0x005A, // Same as ASCII

	KEY_NUMPAD0 = 0x0060,
	KEY_NUMPAD1 = 0x0061,
	KEY_NUMPAD2 = 0x0062,
	KEY_NUMPAD3 = 0x0063,
	KEY_NUMPAD4 = 0x0064,
	KEY_NUMPAD5 = 0x0065,
	KEY_NUMPAD6 = 0x0066,
	KEY_NUMPAD7 = 0x0067,
	KEY_NUMPAD8 = 0x0068,
	KEY_NUMPAD9 = 0x0069,
	KEY_NUMPADMINUS = 0x006D,
	KEY_NUMPADDEC = 0x006E,

	KEY_F1 = 0x0070, // Same as Win32
	KEY_F2 = 0x0071, // Same as Win32
	KEY_F3 = 0x0072, // Same as Win32
	KEY_F4 = 0x0073, // Same as Win32
	KEY_F5 = 0x0074, // Same as Win32
	KEY_F6 = 0x0075, // Same as Win32
	KEY_F7 = 0x0076, // Same as Win32
	KEY_F8 = 0x0077, // Same as Win32
	KEY_F9 = 0x0078, // Same as Win32
	KEY_F10 = 0x0079, // Same as Win32
	KEY_F11 = 0x007A, // Same as Win32
	KEY_F12 = 0x007B, // Same as Win32
};
class InputManagerWin : public InputManager
{
public:
	void onLocalKeyDown(int key);
	void onLocalKeyUp(int key);
	void onLocalMouseDown(int btn, int x, int y);
	void onLocalMouseUp(int btn, int x, int y);

	virtual void update(float dt) override;
protected:
	virtual void mapVirtualBtns() override;
	std::unordered_map<LocalKey,VirtualButton> _VkMAp;
};