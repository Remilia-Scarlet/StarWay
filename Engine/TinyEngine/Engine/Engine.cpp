#include "TinyEngine/precomp.h"
#include "Engine.h"
#include "Graphic/Manager/GraphicMgr.h"
#include "Graphic/gfx/GfxMesh.h"
#include "TinyEngine/Components/MeshComponent.h"
#include "TinyEngine/Components/TextureComponent.h"
#include "TinyEngine/Components/TransformComponent.h"
#include "Graphic/Manager/ConstantBufferManager.h"
#include "Graphic/Manager/ShaderMgr.h"
#include "TinyEngine/Other/TimerManager.h"
#include "TinyEngine/Input/InputManager.h"
#include "TinyEngine/Graphic/GeometryGenerator.h"
#include "TinyEngine/Components/CameraComponent.h"
#include "Graphic/Manager/LightManager.h"
#include "Graphic/Manager/DefaultMgr.h"
#include "Ash/MultiThread/Task.h"

const int DESIRED_FPS = 60;
const int THREAD_POOL_THREAD_NUMBER = 12;

Engine::Engine()
	:_solutionWidth(0)
	,_solutionHeight(0)
	,_desiredFPS(DESIRED_FPS)
	, _currentTime(0)
	, _paused(false)
	, _exit(false)
	, _threadPool(THREAD_POOL_THREAD_NUMBER)
{
}


Engine::~Engine()
{
}

bool Engine::createInstance(int solutionWidth, int solutionHeight, HWND hwnd)
{
	TinyAssert(s_instance == nullptr,"Memory leak! Call destroyInstance() first!");
	if (s_instance)
		return false;
	s_instance = new Engine();
	if(!s_instance || !s_instance->init(solutionWidth, solutionHeight, hwnd))
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

bool Engine::init(int solutionWidth, int solutionHeight, HWND hWnd)
{
	_solutionWidth = solutionWidth;
	_solutionHeight = solutionHeight;
	do
	{
		TINY_BREAK_IF(!TimerManager::createInstance());
		TINY_BREAK_IF(!GraphicMgr::createInstance(_solutionWidth, _solutionHeight, hWnd));
		TINY_BREAK_IF(!ConstantBufferManager::createInstance());
		TINY_BREAK_IF(!ShaderMgr::createInstance());
		TINY_BREAK_IF(!LightManager::createInstance());
		TINY_BREAK_IF(!InputManager::createInstance());
		TINY_BREAK_IF(!DefaultMgr::createInstance());
		return true;
	} while (0);
	return false;
}

void Engine::cleanUp()
{
	_exit = true;
	_currentScene = nullptr;
	DefaultMgr::destroyInstance();
	TimerManager::destoryInstance();
	GraphicMgr::destroyInstance();
	ConstantBufferManager::destroyInstance();
	ShaderMgr::destroyInstance();
	LightManager::destroyInstance();
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

	//game logic
	TaskPtr updateTask = MakeRefCountPtr<Task>(std::bind(&Engine::updateWorld, this, std::placeholders::_1, dt));
	_threadPool.addTask(updateTask);
	_threadPool.waitForAllTasksFinished();

	//game logic
	//updateWorld(dt);

	//draw current scene
	drawScene();

	// input
	InputManager::instance()->update(dt);
}


float Engine::getTime() const
{
	return _currentTime;
}

bool Engine::isExiting() const
{
	return _exit;
}

void Engine::start()
{
}

void Engine::startScene(const ScenePtr& scene)
{
	_currentScene = scene;
	scene->start();
}

ScenePtr Engine::getCurrentScene()
{
	return _currentScene;
}

void Engine::drawScene()
{
	GraphicMgr::instance()->preRender();
	if (_currentScene.isValid())
		_currentScene->render();
	GraphicMgr::instance()->render();
}

void Engine::updateWorld(Task* task, float dt)
{
	TimerManager::instance()->update(dt);
	if (_currentScene.isValid())
		_currentScene->update(task, dt);
}

Engine* Engine::s_instance = nullptr;