#pragma once
#include "Graphic/GfxDefines.h"
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix.h"
#include "Graphic/ShaderInclude/global_constants.h"

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
	virtual void setVSLocalVector(int slot, const Vector4& vector) = 0;
	virtual void setPSLocalVector(int slot, const Vector4& vector) = 0;

	virtual void setVSLocalFloat(int slot, float value) = 0;
	virtual void setPSLocalFloat(int slot, float value) = 0;

	virtual void setVSLocalMatrix(int slot, const Matrix4& matrix) = 0;
	virtual void setPSLocalMatrix(int slot, const Matrix4& matrix) = 0;

	virtual void commitVSBuffer() = 0;
	virtual void commitPSBuffer() = 0;

	virtual void registVSBuffer(int bufferSlot, int size) = 0;
	virtual void registPSBuffer(int bufferSlot, int size) = 0;

	virtual void setRegistVSBuffer(int bufferSlot, void* data) = 0;
	virtual void setRegistPSBuffer(int bufferSlot, void* data) = 0;

	virtual PsGlobalConstantBuffer& getPsGlobalConstant() { return _psConstant; }
	virtual VsGlobalConstantBuffer& getVsGlobalConstant() { return _vsConstant; }
protected:
	virtual bool init() = 0;
	static ConstantBufferManager* s_instance;

	PsGlobalConstantBuffer _psConstant;
	VsGlobalConstantBuffer _vsConstant;
protected:
	ConstantBufferManager();
	virtual ~ConstantBufferManager();
};


#if defined(TINY_PLATFORM_WINDOWS)
	#include "Graphic/DX11/DX11ConstantBufferManager.h"
#else
	#error Unknown graphic engine
#endif