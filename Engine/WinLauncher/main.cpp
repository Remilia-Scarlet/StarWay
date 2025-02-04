#include "WinLauncher/PreComp.h"
#include <Windows.h>
#include <d3d11sdklayers.h>
#include "DangoEngine/Input/InputManager.h"
#include "tools/renderdoc/RenderDoc.h"
#include "StartWayCmdLineCfg/StartWayCmdLineCfg.h"
#include "Game/StarWayGame.h"
#include <crtdbg.h>


static const bool SHOW_WIN32_CONSOLE_AT_START = true;

void ShowHideWin32Console()
{
	static std::vector<char> s_buffer(1024,'\0');
	DWORD num =  GetConsoleTitle(s_buffer.data(), (DWORD)s_buffer.size());
	if (num == 0)
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		printf("Console Started\n");
	}
	else
	{
		HWND hwnd = FindWindow(NULL, s_buffer.data());
		ShowWindow(hwnd,!IsWindowVisible(hwnd));
	}
}

void CloseWin32Console()
{
	FreeConsole();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (int(wParam) == VK_F4)
			ShowHideWin32Console();
		InputManager::instance()->onLocalKeyDown(int(wParam));
		break;
	case WM_KEYUP:
		InputManager::instance()->onLocalKeyUp(int(wParam));
		break;
	case WM_LBUTTONDOWN:
		InputManager::instance()->onLocalMouseDown(1, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		InputManager::instance()->onLocalMouseDown(3, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		InputManager::instance()->onLocalMouseUp(1, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		InputManager::instance()->onLocalMouseUp(3, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND InitWindow(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "TinyDX11EngineClass";
	wcex.hIconSm = 0;
	if (!RegisterClassEx(&wcex))
		return nullptr;

	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hWnd = CreateWindow("TinyDX11EngineClass", "TinyDX11Engin", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);
	if (!hWnd)
		return nullptr;

	ShowWindow(hWnd, nCmdShow);

	return hWnd;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

#ifndef TINY_RETAIL
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
//	_CrtDumpMemoryLeaks();
//	_CrtSetBreakAlloc(617);
#endif

#ifndef TINY_RETAIL
	new int(0x74736574); //to test if the memory leak detector works well
#endif

	if (SHOW_WIN32_CONSOLE_AT_START)
		ShowHideWin32Console();

	using Engine = Dango::Engine;

	std::string cmd = Ash::wcharToChar(lpCmdLine);
	std::unique_ptr<Dango::Game> theGame = ::dangoCreateGameInstance();

	Engine::createInstance(std::move(theGame));
	
	bool result = Engine::instance()->preInit(cmd);
	TinyAssert(result);

	Dango::Game* game = Engine::instance()->getGame();
	auto [desiredWidth, desiredHeight, fps] = game->getGameConfig();
	
	HWND hwnd = InitWindow(hInstance, nCmdShow, desiredWidth, desiredHeight);
	if (!hwnd)
	{
		//(NULL, "Create Window failed.", "Error", MB_OK);
		return 0;
	}

	result = Engine::instance()->init(hwnd);
	TinyAssert(result);

	Engine::instance()->start();

	// Main message loop
	LARGE_INTEGER freq;
	LARGE_INTEGER start_t, stop_t;
	float frameTime;
	float remainTime;
	float desiredFrameTime;
	uint64_t frame = 0;
	double average = 0;
	MSG msg = { 0 };
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start_t);
	while (WM_QUIT != msg.message)
	{
		desiredFrameTime = 1.0f / fps;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		QueryPerformanceCounter(&stop_t);
		frameTime = float(stop_t.QuadPart - start_t.QuadPart) / float(freq.QuadPart);
		if (frameTime >= desiredFrameTime || 1)
		{
			start_t = stop_t;
			++frame;
			Engine::instance()->mainLoop(frameTime);
			QueryPerformanceCounter(&stop_t);
			frameTime = float(stop_t.QuadPart - start_t.QuadPart) / float(freq.QuadPart);
			average += frameTime;
		}
		else
		{
			remainTime = desiredFrameTime - frameTime;
			remainTime *= 1000.0;
			if(remainTime > 0)
				Sleep(DWORD(remainTime));
		}
	}

	Ash::DebugString("Average frame time:%f", average/frame);

	Engine::instance()->cleanUp();

	Engine::destroyInstance();

#if 0
	ID3D11Debug *d3dDebug;
	HRESULT hr = LocalSetting::instance()->_d3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	if (d3dDebug != nullptr)			d3dDebug->Release();
#endif // _DEBUG

	FreeConsole();

	return (int)msg.wParam;
}
