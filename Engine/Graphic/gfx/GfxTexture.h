#pragma once
#include "Ash/RefCountPointer/RefCountObj.h"
#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	class DX11Texture;

	typedef DX11Texture PlatformTexture;
	typedef RefCountPtr<DX11Texture> PlatformTexturePtr;
#else
	#error Unknown platform
#endif


TINY_DEFINE_PTR(GfxTexture);
class GfxTexture : public RefCountObj
{
public:
	static PlatformTexturePtr create(const uint8_t* ddsData, int dataSize,const char* debugName);
	static PlatformTexturePtr create();
public:
	GfxTexture();
	virtual ~GfxTexture();

	virtual void render() = 0;
protected:
	virtual bool init(const uint8_t* ddsData, int dataSize, const char* debugName) = 0;
};

#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	#include "Graphic/DX11/DX11Texture.h"
#else
	#error Unknown platform
#endif