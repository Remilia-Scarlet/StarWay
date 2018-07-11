#pragma once
#include "Ash/RefCountPointer/RefCountObj.h"
#if defined(TINY_PLATFORM_WINDOWS)
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

	virtual bool isWireFrame() = 0;
	virtual void setWireFrame(bool wireFrame) = 0;
	virtual void render() = 0;
protected:
	virtual bool init(const uint8_t* ddsData, int dataSize, const char* debugName) = 0;
};

#if defined(TINY_PLATFORM_WINDOWS)
	#include "Graphic/DX11/DX11Texture.h"
#else
	#error Unknown platform
#endif