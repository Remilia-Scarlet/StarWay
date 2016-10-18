#pragma once
#include "TinyEngine/Scene/Scene.h"
class Engine
{
public:
	static bool createInstance();
	static void destroyInstance();
	static Engine* instance();

	bool start();
	void startScene(ScenePtr scene);
	void end();

	void mainLoop(double dt);

	int getSolutionWidth() { return _solutionWidth; }
	int getSolutionHeight() { return _solutionHeight; }

	int getDesiredFPS() {return _desiredFPS;}
	void setDesiredFPS(int desiredFPS) { _desiredFPS = desiredFPS; }
protected:
	Engine();
	virtual ~Engine();
	bool init();

	void drawScene();
	void updateWorld(double dt);
	static Engine* s_instance;
	int _solutionWidth;
	int _solutionHeight;
	int _desiredFPS;
	ScenePtr _currentScene;
};

