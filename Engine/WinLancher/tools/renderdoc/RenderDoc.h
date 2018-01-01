#pragma once
#include <renderdoc_app.h>


class RenderDoc
{
public:
	static RenderDoc* instance();
	static bool createInstance();
	static void destroy();
protected:
	bool init();
	void clean();

	RENDERDOC_API_1_1_1 * _renderDocApi = nullptr;

	static RenderDoc* s_instance;
};
