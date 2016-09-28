#pragma once

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
#include <windows.h>
#endif

#ifdef _DEBUG
#include <d3d11.h>
#endif // _DEBUG


class LocalSetting
{
public:
	static LocalSetting* instance();

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	HWND getWindowHWND() { return _windowHwnd; }
	void setWindowHWND(HWND windowHwnd) { _windowHwnd = windowHwnd; }
#endif
#ifdef _DEBUG
	ID3D11Device* _d3dDevice = nullptr;
#endif // _DEBUG

protected:
	explicit LocalSetting();
	virtual ~LocalSetting();

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	HWND _windowHwnd;
#endif
};

