#include "precomp.h"
#include "StartWayCmdLineCfg.h"
StartWayCmdLineCfg* StartWayCmdLineCfg::s_instance = nullptr;

bool StartWayCmdLineCfg::createInstance(const char* commandLine)
{
	StartWayCmdLineCfg* instance = new StartWayCmdLineCfg();
	if (!instance || !instance->init(commandLine))
	{
		TINY_SAFE_DELETE(instance);
		TinyAssert(false, "CommandLineCfg init failed. Please check commandline, use /? to get all commanline");
		return false;
	}
	s_instance = instance;
	return true;
}

StartWayCmdLineCfg* StartWayCmdLineCfg::instance()
{
	TinyAssert(s_instance, "CommandLineCfg haven't been initialized");
	return s_instance;
}

void StartWayCmdLineCfg::destroy()
{
	TINY_SAFE_DELETE(s_instance);
}