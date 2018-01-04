#pragma once
#include "Graphic/GfxDefines.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "TinyEngine/Engine/EngineDefs.h"

TINY_DEFINE_PTR(GfxRenderBuffer);
class GfxRenderBuffer : public RefCountObj
{
protected:
	GfxRenderBuffer();
	virtual ~GfxRenderBuffer();
public:
	static GfxRenderBufferPtr create(int width, int height);
public:
	virtual void render() = 0;
private:
	virtual bool init(int width, int height) = 0;
};


