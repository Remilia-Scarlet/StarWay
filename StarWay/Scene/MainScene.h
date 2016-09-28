#pragma once
TINY_DEFINE_PTR(MainScene);

class MainScene : public Scene
{
public:
	static MainScenePtr create();

	virtual bool start() override;
protected:
	virtual bool init() override;
};
