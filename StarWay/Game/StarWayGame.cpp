#include "StarWay/precomp.h"
#include "StarWayGame.h"
#include "StarWay/StartWayCmdLineCfg/StartWayCmdLineCfg.h"
#include "StarWay/Scene/MainScene.h"

StarWayGame::StarWayGame()
{
}

StarWayGame::~StarWayGame()
{
}

bool StarWayGame::preInit(const char* cmdLine)
{
	return StartWayCmdLineCfg::createInstance(cmdLine);
}

bool StarWayGame::init(int solutionWidth, int solutionHeight, HWND hwnd)
{
	return Engine::createInstance(solutionWidth, solutionHeight, hwnd);
}

bool StarWayGame::start()
{
	Engine::instance()->start();
	MainScenePtr mainScene = MainScene::create();
	Engine::instance()->startScene(mainScene);
	return true;
}

bool StarWayGame::mainLoop(float delta)
{
	Engine::instance()->mainLoop(delta);
	return true;
}

bool StarWayGame::cleanUp()
{
	Engine::instance()->cleanUp();
	return true;
}

bool StarWayGame::destroy()
{
	Engine::destroyInstance();
	StartWayCmdLineCfg::destroy();
	return true;
}

int StarWayGame::getDesiredFPS()
{
	return Engine::instance()->getDesiredFPS();
}
