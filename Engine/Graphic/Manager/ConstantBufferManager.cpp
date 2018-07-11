#include "precomp.h"
#include "ConstantBufferManager.h"

ConstantBufferManager* ConstantBufferManager::s_instance = nullptr;
PlatformConstantBufferManager* ConstantBufferManager::instance()
{
	TinyAssert(s_instance != nullptr, "Call createInstance first");
	return static_cast<PlatformConstantBufferManager*>(s_instance);
}

bool ConstantBufferManager::createInstance()
{
	TinyAssert(s_instance == nullptr, "Call destroyInstance first");
#if defined(TINY_PLATFORM_WINDOWS)
	s_instance = new DX11ConstantBufferManager();
#else
#error unkonwn platform
#endif
	if (!s_instance || !s_instance->init())
	{
		TinyAssert(false, "Create ConstantBufferManager failed!");
		TINY_SAFE_DELETE(s_instance);
	}
	return s_instance != nullptr;
}

bool ConstantBufferManager::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
	return true;
}

ConstantBufferManager::ConstantBufferManager()
{

}


ConstantBufferManager::~ConstantBufferManager()
{
}
