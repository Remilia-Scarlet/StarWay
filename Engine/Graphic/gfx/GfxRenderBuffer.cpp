#include "precomp.h"
#include "GfxRenderBuffer.h"

#if defined(TINY_PLATFORM_WINDOWS)
	#include "Graphic/DX11/DX11RenderBuffer.h"
	class DX11RenderBuffer;

	typedef DX11RenderBuffer PlatformRenderBuffer;
	typedef RefCountPtr<DX11RenderBuffer> PlatformRenderBufferPtr;
#else
	#error Unknown platform
#endif

GfxRenderBuffer::GfxRenderBuffer()
{

}

GfxRenderBuffer::~GfxRenderBuffer()
{

}

GfxRenderBufferPtr GfxRenderBuffer::create(int width, int height)
{
	GfxRenderBuffer* ret = new PlatformRenderBuffer();
	if (ret && ret->init(width, height))
		return GfxRenderBufferPtr(ret);
	TINY_SAFE_DELETE(ret);
	return nullptr;
}
