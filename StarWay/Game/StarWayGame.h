#pragma once
#include "DangoEngine/Engine/Game.h"
#include "StartWayCmdLineCfg/StartWayCmdLineCfg.h"

namespace StartWay
{
	class StarWayGame : public Dango::Game
	{
	public:
		StarWayGame();
		virtual ~StarWayGame() override;
		virtual bool preInit(const std::string& cmdLine) override;
		virtual bool init(Ash::NativeWindow hwnd) override;
		virtual bool start() override;
		virtual bool cleanUp() override;
		virtual bool destroy() override;
	protected:
		StartWayCmdLineCfg _cmdLine;
	};
}
DEFINE_GAME(StartWay::StarWayGame);