#include "Graphic\precomp.h"
#include "GfxTexture.h"


PlatformTexturePtr GfxTexture::create(const uint8_t* ddsData, int dataSize, const char* debugName)
{
	PlatformTexture* ret = new PlatformTexture();
	if (!ret || !static_cast<GfxTexture*>(ret)->init(ddsData, dataSize, debugName))
		TINY_SAFE_DELETE(ret);
	return PlatformTexturePtr(ret);
}

PlatformTexturePtr GfxTexture::create()
{
	PlatformTexture* ret = new PlatformTexture();
	if (!ret || !static_cast<GfxTexture*>(ret)->init(nullptr, 0, ""))
		TINY_SAFE_DELETE(ret);
	return PlatformTexturePtr(ret);
}

GfxTexture::GfxTexture()
{
}


GfxTexture::~GfxTexture()
{
}
