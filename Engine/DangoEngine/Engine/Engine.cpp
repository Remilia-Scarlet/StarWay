#include "DangoEngine/precomp.h"
#include "Engine.h"
#include "DangoEngine/Engine/Game.h"
#include "Graphic/Manager/GraphicMgr.h"
#include "Graphic/gfx/GfxMesh.h"
#include "DangoEngine/Components/MeshComponent.h"
#include "DangoEngine/Components/MaterialComponent.h"
#include "DangoEngine/Components/TransformComponent.h"
#include "Graphic/Manager/ConstantBufferManager.h"
#include "Graphic/Manager/ShaderMgr.h"
#include "DangoEngine/Other/TimerManager.h"
#include "DangoEngine/Input/InputManager.h"
#include "DangoEngine/Graphic/GeometryGenerator.h"
#include "DangoEngine/Components/CameraComponent.h"
#include "Graphic/Manager/LightManager.h"
#include "Graphic/Manager/DefaultMgr.h"


Dango::Engine::Engine()
	: _paused(false)
	, _exit(false)
{
}


Dango::Engine::~Engine()
{
}

bool Dango::Engine::createInstance(std::unique_ptr<Game> game)
{
	TinyAssert(s_instance == nullptr,"Memory leak! Call destroyInstance() first!");
	TinyAssert(game, "None game set");
	if (s_instance)
		return false;
	s_instance = new Engine();
	s_instance->_game = std::move(game);
	return true;
}

Dango::Engine* Dango::Engine::instance()
{
	TinyAssert(s_instance != nullptr,"No instance!! Call createInstance() first!");
	return s_instance;
}

bool Dango::Engine::preInit(const std::string& cmdLine)
{
	TinyAssert(_game);
	do
	{
		TINY_BREAK_IF(_game->preInit(cmdLine));
		return true;
	} while (false);
	return false;
}

bool Dango::Engine::init(Ash::NativeWindow nativeWindow)
{
	_game->init(nativeWindow);
	do
	{
		TINY_BREAK_IF(!TimerManager::createInstance());
		TINY_BREAK_IF(!GraphicMgr::createInstance(_solutionWidth, _solutionHeight, nativeWindow));
		TINY_BREAK_IF(!ConstantBufferManager::createInstance());
		TINY_BREAK_IF(!ShaderMgr::createInstance());
		TINY_BREAK_IF(!LightManager::createInstance());
		TINY_BREAK_IF(!InputManager::createInstance());
		TINY_BREAK_IF(!DefaultMgr::createInstance());
		return true;
	} while (0);
	return false;
}

void Dango::Engine::cleanUp()
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
void Dango::Engine::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
}

void Dango::Engine::mainLoop(float dt)
{
	if (_paused)
		return;

	Ash::Semaphore sem;
	Ash::FunctorSeq::entry([this, dt, &sem](Ash::FunctorSeq& seq)
	{
		seq.then(std::bind(&Engine::updateManager, std::placeholders::_1, dt));
		seq.then(std::bind(&Engine::updateWorld, std::placeholders::_1, dt));
		seq.then(std::bind(&Engine::drawScene, std::placeholders::_1, dt));
		seq.then([&sem](Ash::FunctorSeq& seq)
		{
			sem.release();
		});
	});
	sem.acquire();
}

bool Dango::Engine::isExiting() const
{
	return _exit;
}

void Dango::Engine::start()
{
	_game->start();
}

void Dango::Engine::startScene(const ScenePtr& scene)
{
	_currentScene = scene;
	scene->start();
}

ScenePtr Dango::Engine::getCurrentScene()
{
	return _currentScene;
}

void Dango::Engine::drawScene(Ash::FunctorSeq& seq, float dt)
{
	GraphicMgr::instance()->preRender();
	if (_currentScene.isValid())
		_currentScene->render();
	GraphicMgr::instance()->render();
}

void Dango::Engine::updateManager(Ash::FunctorSeq& seq, float dt)
{
	// input
	InputManager::instance()->update(dt);

}

void Dango::Engine::updateWorld(Ash::FunctorSeq& seq, float dt)
{
	seq.then([this, dt](Ash::FunctorSeq& seq)
	{
		if (_currentScene.isValid())
			_currentScene->update(seq, dt);
	});
	
}

Dango::Engine* Dango::Engine::s_instance = nullptr;