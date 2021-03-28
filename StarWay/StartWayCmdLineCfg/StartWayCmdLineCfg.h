#pragma once
#include "Ash/CommandLineCfg/CommandLineCfg.h"

namespace StartWay
{
	class StartWayCmdLineCfg : public Ash::CommandLineCfg
	{
	public:
		DEFINE_COMMANDLINE_BOOL(Renderdoc, "Enable renderdoc in-app api");
		//DEFINE_COMMANDLINE_INT(Someint, 233, "SomeIntDesc");
		//DEFINE_COMMANDLINE_STR(Somestr, "233", "SomestrDesc");
	};
}