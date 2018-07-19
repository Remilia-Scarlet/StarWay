#pragma once
#include "TinyEngine/Scene/Scene.h"
#include "Ash/MultiThread/ThreadPool.h"

#if defined(TINY_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

class Engine
{
public:
	static bool createInstance(int solutionWidth, int solutionHeight, HWND hwnd);
	static void destroyInstance();
	static Engine* instance();

	void start();
	void startScene(const ScenePtr& scene);
	ScenePtr getCurrentScene();
	void cleanUp();

	void mainLoop(float dt);

	int getSolutionWidth() { return _solutionWidth; }
	int getSolutionHeight() { return _solutionHeight; }

	int getDesiredFPS() {return _desiredFPS;}
	void setDesiredFPS(int desiredFPS) { _desiredFPS = desiredFPS; }

	// return time in seconds since game started
	float getTime() const;

	bool isExiting() const;
protected:
	Engine();
	virtual ~Engine();
	bool init(int solutionWidth, int solutionHeight
#if defined(TINY_PLATFORM_WINDOWS)
		,HWND hWnd
#endif
	);

	void drawScene();
	void updateWorld(Task* task, float dt);
	static Engine* s_instance;
	int _solutionWidth;
	int _solutionHeight;
	int _desiredFPS;
	ScenePtr _currentScene;
	float _currentTime;
	bool _paused;
	bool _exit;
	ThreadPool _threadPool;
};

