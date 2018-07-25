#pragma once
#include "Graphic/Manager/ConstantBufferManager.h"
class DX11ConstantBufferManager : public ConstantBufferManager
{
public:
	const int CONSTANT_SLOT_NUMBER;
public:
	DX11ConstantBufferManager();
	virtual ~DX11ConstantBufferManager();
public:
	virtual void setVSLocalVector(int slot, const Vector4& vector)override;
	virtual void setPSLocalVector(int slot, const Vector4& vector)override;

	virtual void setVSLocalFloat(int slot, float value)override;
	virtual void setPSLocalFloat(int slot, float value)override;

	virtual void setVSLocalMatrix(int slot, const Matrix4& matrix) override;
	virtual void setPSLocalMatrix(int slot, const Matrix4& matrix) override;

	virtual void commitVSBuffer()override;
	virtual void commitPSBuffer()override;

	virtual void registVSBuffer(int bufferSlot, int size)override;
	virtual void registPSBuffer(int bufferSlot, int size)override;

	virtual void setRegistVSBuffer(int bufferSlot, void* data)override;
	virtual void setRegistPSBuffer(int bufferSlot, void* data)override;
protected:
	virtual bool init()override;
	std::vector<Vector4> _tempVSLocalBuffer;
	std::vector<Vector4> _tempPSLocalBuffer;
	ID3D11Buffer* _vsLocalBuffer; //slot 0
	ID3D11Buffer* _psLocalBuffer; //slot 0
	ID3D11Buffer* _vsGlobalBuffer; //slot 1
	ID3D11Buffer* _psGlobalBuffer; //slot 1

	std::unordered_map<int, ID3D11Buffer*> _registVSBufferMap;
	std::unordered_map<int, ID3D11Buffer*> _registPSBufferMap;
};

