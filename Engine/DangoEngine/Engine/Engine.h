#pragma once
#include "DangoEngine/Scene/Scene.h"
#include "Ash/MultiThread/ThreadPool.h"

#if defined(TINY_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace Dango
{
	class Game;

	class Engine
	{
	public:
		static bool createInstance(std::unique_ptr<Game> game);
		static void destroyInstance();
		static Engine* instance();

		bool preInit(const std::string& cmdLine);
		bool init(Ash::NativeWindow nativeWindow);
		
		void start();
		void startScene(const ScenePtr& scene);
		ScenePtr getCurrentScene();
		void cleanUp();

		void mainLoop(float dt);

		Game* getGame() const { return _game.get(); }

		bool isExiting() const;
	protected:
		Engine();
		virtual ~Engine();

		void drawScene(Ash::FunctorSeq& seq, float dt);
		void updateManager(Ash::FunctorSeq& seq, float dt);
		void updateWorld(Ash::FunctorSeq& seq, float dt);
		static Engine* s_instance;

		ScenePtr _currentScene;
		bool _paused;
		bool _exit;
		std::unique_ptr<Game> _game;
	};

}