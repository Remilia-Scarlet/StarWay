#pragma once
#include "Graphic/Vertex/InputLayoutDefine.h"
#include "Ash/RefCountPointer/RefCountObj.h"

#if defined(TINY_PLATFORM_WINDOWS)
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

