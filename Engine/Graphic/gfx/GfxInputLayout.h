#pragma once
#include "Graphic/Vertex/InputLayoutDefine.h"
#include "Ash/RefCountPointer/RefCountObj.h"

#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	class DX11InputLayout;

	typedef DX11InputLayout PlatformInputLayout;
	typedef RefCountPtr<DX11InputLayout> PlatformInputLayoutPtr;
#else
	#error Unknown platform
#endif



TINY_DEFINE_PTR(GfxInputLayout);
class GfxInputLayout : public RefCountObj
{
public:
	virtual void render() = 0;
protected:
	GfxInputLayout();
	virtual ~GfxInputLayout();
};

