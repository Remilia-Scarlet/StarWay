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
	virtual void setVSVector(int slot, const Vector4& vector)override;
	virtual void setPSVector(int slot, const Vector4& vector)override;

	virtual void setVSFloat(int slot, float value)override;
	virtual void setPSFloat(int slot, float value)override;

	virtual void setVSMatrix(int slot, const Matrix4& matrix) override;
	virtual void setPSMatrix(int slot, const Matrix4& matrix) override;

	virtual void commitVSBuffer()override;
	virtual void commitPSBuffer()override;
protected:
	virtual bool init()override;
	std::vector<Vector4> _tempVSSubmitBuffer;
	std::vector<Vector4> _tempPSSubmitBuffer;
	ID3D11Buffer* _vsConstantBuffer;
	ID3D11Buffer* _psConstantBuffer;
};

