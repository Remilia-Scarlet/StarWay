#pragma once
#include "TinyEngine/Engine/Game.h"

class StarWayGame : public Game
{
public:
	StarWayGame();
	virtual ~StarWayGame() override;
	virtual bool preInit(const char* cmdLine) override;
	virtual bool init(int solutionWidth, int solutionHeight, HWND hwnd) override;
	virtual bool start() override;
	virtual bool mainLoop(float delta) override;
	virtual bool cleanUp() override;
	virtual bool destroy() override;

public:
	int getDesiredFPS();
};
