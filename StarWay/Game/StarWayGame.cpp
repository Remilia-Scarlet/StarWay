#include "precomp.h"
#include "StarWayGame.h"
#include "StartWayCmdLineCfg/StartWayCmdLineCfg.h"
#include "Scene/MainScene.h"

StartWay::StarWayGame::StarWayGame()
{
}

StartWay::StarWayGame::~StarWayGame()
{
}

bool StartWay::StarWayGame::preInit(const std::string& cmdLine)
{
	_gameConfig.width = 1024;
	_gameConfig.height = 768;
	_gameConfig.fps = 30;
	do
	{
		TINY_BREAK_IF(_cmdLine.init(cmdLine.c_str()));
		return true;
	} while (false);
	return false;
}

bool StartWay::StarWayGame::init(Ash::NativeWindow hwnd)
{
	return true;
}

bool StartWay::StarWayGame::start()
{
	StarWay::MainScenePtr mainScene = StarWay::MainScene::create();
	Dango::Engine::instance()->startScene(mainScene);
	return true;
}

bool StartWay::StarWayGame::cleanUp()
{
	return true;
}

bool StartWay::StarWayGame::destroy()
{
	return true;
}
