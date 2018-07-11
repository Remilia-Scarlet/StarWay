#pragma once
#include "Graphic/GfxDefines.h"
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix.h"

#if defined(TINY_PLATFORM_WINDOWS)
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

	virtual void registVSBuffer(int bufferSlot, int size) = 0;
	virtual void registPSBuffer(int bufferSlot, int size) = 0;

	virtual void setRegistVSBuffer(int bufferSlot, void* data) = 0;
	virtual void setRegistPSBuffer(int bufferSlot, void* data) = 0;
protected:
	virtual bool init() = 0;
	static ConstantBufferManager* s_instance;
protected:
	ConstantBufferManager();
	virtual ~ConstantBufferManager();
};


#if defined(TINY_PLATFORM_WINDOWS)
	#include "Graphic/DX11/DX11ConstantBufferManager.h"
#else
	#error Unknown graphic engine
#endif