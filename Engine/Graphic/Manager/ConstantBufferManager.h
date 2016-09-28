#pragma once
#include "Graphic/GfxDefines.h"
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix4.h"

#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	class DX11ConstantBufferManager;
	typedef DX11ConstantBufferManager PlatformConstantBufferManager;
#else
	#error Unknown graphic engine
#endif

class ConstantBufferManager
{
public:
	static PlatformConstantBufferManager* instance();
	static bool createInstance();
	static bool destroyInstance();
public:
	virtual void setVSVector(int slot, const Vector4& vector) = 0;
	virtual void setPSVector(int slot, const Vector4& vector) = 0;

	virtual void setVSFloat(int slot, float value) = 0;
	virtual void setPSFloat(int slot, float value) = 0;

	virtual void setVSMatrix(int slot, const Matrix4& matrix) = 0;
	virtual void setPSMatrix(int slot, const Matrix4& matrix) = 0;

	virtual void commitVSBuffer() = 0;
	virtual void commitPSBuffer() = 0;
protected:
	virtual bool init() = 0;
	static ConstantBufferManager* s_instance;
protected:
	ConstantBufferManager();
	virtual ~ConstantBufferManager();
};


#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	#include "Graphic/DX11/DX11ConstantBufferManager.h"
#else
	#error Unknown graphic engine
#endif