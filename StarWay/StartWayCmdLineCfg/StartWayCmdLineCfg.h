#pragma once
#include "Ash/CommandLineCfg/CommandLineCfg.h"

class StartWayCmdLineCfg : public Ash::CommandLineCfg
{
public:
	static bool createInstance(const char* commandLine);
	static StartWayCmdLineCfg* instance();
	static void destroy();
protected:
	static StartWayCmdLineCfg* s_instance;
public:
	DEFINE_COMMANDLINE_BOOL(Renderdoc, "Enable renderdoc in-app api");
	//DEFINE_COMMANDLINE_INT(Someint, 233, "SomeIntDesc");
	//DEFINE_COMMANDLINE_STR(Somestr, "233", "SomestrDesc");
};
