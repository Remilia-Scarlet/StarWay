#include "Graphic/precomp.h"
#include "GraphicMgr.h"

#if defined(TINY_PLATFORM_WINDOWS)
#include "Graphic/DX11/DX11GraphicMgr.h"
#endif


GraphicMgr* GraphicMgr::s_instance = nullptr;

PlatformGraphicMgr* GraphicMgr::instance()
{
	TinyAssert(s_instance != nullptr, "No instance!! Call createInstance first");
	return static_cast<PlatformGraphicMgr*>(s_instance);
}

bool GraphicMgr::createInstance(int width, int height, HWND hWnd)
{
#if defined(TINY_PLATFORM_WINDOWS)
	PlatformGraphicMgr* instan = new PlatformGraphicMgr();
	if (!instan || !instan->init(width,height,hWnd))
	{
		TinyAssert(false, "Create GraphicMgr failed!");
		if (instan)
			delete instan;
		return false;
	}
	s_instance = instan;
#else
	#error Unknown Platform
#endif
	return true;
}
void GraphicMgr::destroyInstance()
{
	if (s_instance)
	{
		s_instance->clearDevice();
		delete s_instance;
	}
	s_instance = nullptr;
}
GraphicMgr::GraphicMgr()
{
}


GraphicMgr::~GraphicMgr()
{
}
