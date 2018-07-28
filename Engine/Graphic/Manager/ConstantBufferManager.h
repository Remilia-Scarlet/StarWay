#pragma once
#include "Graphic/GfxDefines.h"
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix.h"
#include "Graphic/ShaderInclude/global_constants.h"
#include "Graphic/ShaderInclude/ShaderInfo.h"

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
	template<class T>
	void setLocalConstantVs(const LocalConstantVs& constant, const T& data);
	template<class ValueType, int RowNum, int ColNum>
	void setLocalConstantVs(const LocalConstantVs& constant, const MatrixStorage<ValueType,RowNum,ColNum>& data);
	void setLocalConstantVs(const LocalConstantVs& constant, const void* data, size_t size);

	template<class T>
	void setLocalConstantPs(const LocalConstantPs& constant, const T& data);
	template<class ValueType, int RowNum, int ColNum>
	void setLocalConstantPs(const LocalConstantVs& constant, const MatrixStorage<ValueType, RowNum, ColNum>& data);
	void setLocalConstantPs(const LocalConstantPs& constant, const void* data, size_t size);

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

	std::vector<Vector4> _tempVSLocalBuffer;
	std::vector<Vector4> _tempPSLocalBuffer;
protected:
	ConstantBufferManager();
	virtual ~ConstantBufferManager();
};

template <class T>
void ConstantBufferManager::setLocalConstantVs(const LocalConstantVs& constant, const T& data)
{
	setLocalConstantVs(constant, &data, static_cast<int>(sizeof(data)));
}

template <class ValueType, int RowNum, int ColNum>
void ConstantBufferManager::setLocalConstantVs(const LocalConstantVs& constant,
	const MatrixStorage<ValueType, RowNum, ColNum>& data)
{
#if defined(TINY_GRAPHIC_ENGINE_DX11)
	setLocalConstantVs(constant, data.transpose().getData(), sizeof(MatrixStorage<ValueType, RowNum, ColNum>));
#else
	setLocalConstantVs(constant, data.getData(), sizeof(MatrixStorage<ValueType, RowNum, ColNum>));
#endif
}

template <class T>
void ConstantBufferManager::setLocalConstantPs(const LocalConstantPs& constant, const T& data)
{
	setLocalConstantPs(constant, &data, static_cast<int>(sizeof(data)));
}

template <class ValueType, int RowNum, int ColNum>
void ConstantBufferManager::setLocalConstantPs(const LocalConstantVs& constant,
	const MatrixStorage<ValueType, RowNum, ColNum>& data)
{
#if defined(TINY_GRAPHIC_ENGINE_DX11)
	setLocalConstantPs(constant, data.transpose().getData(), sizeof(MatrixStorage<ValueType, RowNum, ColNum>));
#else
	setLocalConstantPs(constant, data.getData(), sizeof(MatrixStorage<ValueType, RowNum, ColNum>));
#endif
}


#if defined(TINY_PLATFORM_WINDOWS)
	#include "Graphic/DX11/DX11ConstantBufferManager.h"
#else
	#error Unknown graphic engine
#endif