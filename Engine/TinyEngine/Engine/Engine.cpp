#include "TinyEngine/precomp.h"
#include "Engine.h"
#include "Graphic/Manager/GraphicMgr.h"
#include "LocalSetting.h"
#include "Graphic/gfx/GfxMesh.h"
#include "TinyEngine/Components/MeshComponent.h"
#include "TinyEngine/Components/TextureComponent.h"
#include "TinyEngine/Components/TransformComponent.h"
#include "Graphic/Manager/ConstantBufferManager.h"
#include "Graphic/Manager/ShaderMgr.h"
#include "TinyEngine/Other/TimerManager.h"
#include "TinyEngine/Input/InputManager.h"
#include "TinyEngine/ScriptManager/LuaManager.h"

const int SOLUTION_WIDTH = 640;
const int SOLUTION_HEIGHT = 480;
const int DESIRED_FPS = 60;

Engine::Engine()
	:_solutionWidth(0)
	,_solutionHeight(0)
	,_desiredFPS(DESIRED_FPS)
	, _currentTime(0)
	, _paused(false)
{
}


Engine::~Engine()
{
}

bool Engine::createInstance()
{
	TinyAssert(s_instance == nullptr,"Memory leak! Call destroyInstance() first!");
	if (s_instance)
		return false;
	s_instance = new Engine();
	if(!s_instance || !s_instance->init())
	{
		TinyAssert(false, "create Engine failed");
		TINY_SAFE_DELETE(s_instance);
		return false;
	}
	return true;
}

Engine* Engine::instance()
{
	TinyAssert(s_instance != nullptr,"No instance!! Call createInstance() first!");
	return s_instance;
}

bool Engine::init()
{
	_solutionWidth = SOLUTION_WIDTH;
	_solutionHeight = SOLUTION_HEIGHT;
	do
	{
		return true;
	} while (0);
	return false;
}

bool Engine::createManagers()
{
	do
	{
		TINY_BREAK_IF(!TimerManager::createInstance());
		TINY_BREAK_IF(!GraphicMgr::createInstance(_solutionWidth, _solutionHeight, LocalSetting::instance()->getWindowHWND()));
		TINY_BREAK_IF(!ConstantBufferManager::createInstance());
		TINY_BREAK_IF(!ShaderMgr::createInstance());
		TINY_BREAK_IF(!InputManager::createInstance());
		TINY_BREAK_IF(!LuaManager::createInstance());
		return true;
	} while (0);
	return false;
}
void Engine::cleanUp()
{
	LuaManager::destroyInstance();
	TimerManager::destoryInstance();
	GraphicMgr::destroyInstance();
	ConstantBufferManager::destroyInstance();
	ShaderMgr::destroyInstance();
	InputManager::destroyInstance();
}
void Engine::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
}

void Engine::mainLoop(float dt)
{
	if (_paused)
		return;
	_currentTime += dt;
	
	// input
	InputManager::instance()->update(dt);

	//game logic
	updateWorld(dt);

	//draw current scene
	drawScene();
}


float Engine::getTime() const
{
	return _currentTime;
}

void Engine::start()
{
	LuaManager::instance()->call("start");
}

void Engine::drawScene()
{
	GraphicMgr::instance()->preRender();
	if (_currentScene.isValid())
		_currentScene->render();
	GraphicMgr::instance()->render();
}

void Engine::updateWorld(float dt)
{
	TimerManager::instance()->update(dt);
	if (_currentScene.isValid())
		_currentScene->update(dt);
}

Engine* Engine::s_instance = nullptr;