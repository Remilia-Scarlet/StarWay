#pragma once
#include "TinyEngine/Scene/Scene.h"
TINY_DEFINE_PTR(MainScene);

class MainScene : public Scene
{
public:
	static MainScenePtr create();
protected:
	virtual bool init() override;

	void createGround();
	void createObj();
	void createApple();
	void createDangoCube();
	void createFreeCamera();
	void createLight();
};