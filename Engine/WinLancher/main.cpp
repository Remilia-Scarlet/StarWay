#include "WinLancher/precomp.h"
#include <windows.h>
#include "TinyEngine/TinyEngine.h"
#include "StarWay/Scene/MainScene.h"
#include <d3d11sdklayers.h>
#include "TinyEngine/Engine/LocalSetting.h"

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
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
//	_CrtDumpMemoryLeaks();
//	_CrtSetBreakAlloc(196);
#endif

#ifdef _DEBUG
	new int(0x74736574); //to test if the memory leak detector works well
#endif

	if (!Engine::createInstance())
	{
		//(NULL,"Create Engine failed.", "Error", MB_OK);
		return 0;
	}

	HWND hwnd = InitWindow(hInstance, nCmdShow, Engine::instance()->getSolutionWidth(), Engine::instance()->getSolutionHeight());
	if (!hwnd)
	{
		//(NULL, "Create Window failed.", "Error", MB_OK);
		return 0;
	}

	//store platform value
	LocalSetting::instance()->setWindowHWND(hwnd);

	//start
	if (!Engine::instance()->start())
	{
		//(NULL, "Init Engine failed.", "Error", MB_OK);
		return 0;
	}

	{
		//create Scene
		ScenePtr scene = MainScene::create();
		Engine::instance()->startScene(scene);
	}
	// Main message loop
	LARGE_INTEGER freq;
	LARGE_INTEGER start_t, stop_t;
	double frameTime;
	double remainTime;
	double desiredFrameTime;
	MSG msg = { 0 };
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start_t);
	while (WM_QUIT != msg.message)
	{
		desiredFrameTime = 1.0 / Engine::instance()->getDesiredFPS();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		QueryPerformanceCounter(&stop_t);
		frameTime = double(stop_t.QuadPart - start_t.QuadPart) / double(freq.QuadPart);
		if (frameTime >= desiredFrameTime)
		{
			start_t = stop_t;
			Engine::instance()->mainLoop(frameTime);
#if _DEBUG
			extern void UnitTest();
			UnitTest();
#endif
		}
		else
		{
			remainTime = desiredFrameTime - frameTime;
			remainTime *= 1000.0;
			if(remainTime > 0)
				Sleep(DWORD(remainTime));
		}
	}

	Engine::instance()->end();

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


	return (int)msg.wParam;
}