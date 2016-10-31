#pragma once
#include "TinyEngine/Scene/Scene.h"
class Engine
{
public:
	static bool createInstance();
	static void destroyInstance();
	static Engine* instance();

	bool createManagers();
	void start(ScenePtr scene);
	void cleanUp();

	void mainLoop(float dt);

	int getSolutionWidth() { return _solutionWidth; }
	int getSolutionHeight() { return _solutionHeight; }

	int getDesiredFPS() {return _desiredFPS;}
	void setDesiredFPS(int desiredFPS) { _desiredFPS = desiredFPS; }

	// return time in seconds since game started
	float getTime() const;
protected:
	Engine();
	virtual ~Engine();
	bool init();

	void drawScene();
	void updateWorld(float dt);
	static Engine* s_instance;
	int _solutionWidth;
	int _solutionHeight;
	int _desiredFPS;
	ScenePtr _currentScene;
	float _currentTime;
	bool _paused;
};

