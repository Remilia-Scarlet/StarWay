#pragma once

#if defined(TINY_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

class Game
{
public:
	Game() = default;
	virtual ~Game() = default;

public:
	//once game start, call this. Before anyothers
	virtual bool preInit(const char* cmdLine) = 0;
	//window created call this
	virtual bool init(int solutionWidth, int solutionHeight, HWND hwnd) = 0;
	
	virtual bool start() = 0;

	virtual bool mainLoop(float delta) = 0;

	virtual bool cleanUp() = 0;

	virtual bool destroy() = 0;
};

