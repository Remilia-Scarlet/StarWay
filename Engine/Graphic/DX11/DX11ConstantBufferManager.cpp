#include "Graphic\precomp.h"
#include "DX11ConstantBufferManager.h"
#include "DX11GraphicMgr.h"
#include "Math\vector\Vector.h"


DX11ConstantBufferManager::DX11ConstantBufferManager()
	: _vsConstantBuffer(nullptr)
	, _psConstantBuffer(nullptr)
	, CONSTANT_SLOT_NUMBER(D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT/sizeof(Vector4))
{
	static_assert(sizeof(Vector4) == 16, "How can sizeof float4 != 16?");
	_tempVSSubmitBuffer.resize(CONSTANT_SLOT_NUMBER);
	_tempPSSubmitBuffer.resize(CONSTANT_SLOT_NUMBER);
}


DX11ConstantBufferManager::~DX11ConstantBufferManager()
{
	TINY_SAFE_RELEASE(_vsConstantBuffer);
	TINY_SAFE_RELEASE(_psConstantBuffer);
}


void DX11ConstantBufferManager::setVSVector(int slot, const Vector4& vector)
{
	TinyAssert(slot < CONSTANT_SLOT_NUMBER);
	if (slot < CONSTANT_SLOT_NUMBER)
	{
		_tempVSSubmitBuffer[slot] = vector;
	}
}

void DX11ConstantBufferManager::setPSVector(int slot, const Vector4& vector)
{
	TinyAssert(slot < CONSTANT_SLOT_NUMBER);
	if (slot < CONSTANT_SLOT_NUMBER)
	{
		_tempPSSubmitBuffer[slot] = vector;
	}
}

void DX11ConstantBufferManager::setVSFloat(int slot, float value)
{
	TinyAssert(slot < CONSTANT_SLOT_NUMBER);
	if (slot < CONSTANT_SLOT_NUMBER)
	{
		_tempVSSubmitBuffer[slot] = value;
	}
}

void DX11ConstantBufferManager::setPSFloat(int slot, float value)
{
	TinyAssert(slot < CONSTANT_SLOT_NUMBER);
	if (slot < CONSTANT_SLOT_NUMBER)
	{
		_tempPSSubmitBuffer[slot] = value;
	}
}

void DX11ConstantBufferManager::setVSMatrix(int slot, const Matrix4& matrix)
{
	Matrix4 transMatrix = matrix.transpose();
	TinyAssert(slot + 4< CONSTANT_SLOT_NUMBER);
	if (slot + 4 < CONSTANT_SLOT_NUMBER)
	{
		for (int i = 0; i < 4; ++i)
		{
			_tempVSSubmitBuffer[slot + i] = transMatrix(i);
		}
	}
}

void DX11ConstantBufferManager::setPSMatrix(int slot, const Matrix4& matrix)
{
	TinyAssert(slot + 4 < CONSTANT_SLOT_NUMBER);
	if (slot + 4 < CONSTANT_SLOT_NUMBER)
	{
		for (int i = 0; i < 4; ++i)
		{
			_tempPSSubmitBuffer[slot + i] = matrix(i);
		}
	}

}

void DX11ConstantBufferManager::commitVSBuffer()
{
	ID3D11DeviceContext* d3dContent = GraphicMgr::instance()->getD3D11DeviceContext();
	d3dContent->UpdateSubresource(_vsConstantBuffer, 0, NULL, _tempVSSubmitBuffer.data(), 0, 0);
	d3dContent->VSSetConstantBuffers(0, 1, &_vsConstantBuffer);
}

void DX11ConstantBufferManager::commitPSBuffer()
{
	ID3D11DeviceContext* d3dContent = GraphicMgr::instance()->getD3D11DeviceContext();
	d3dContent->UpdateSubresource(_psConstantBuffer, 0, NULL, _tempPSSubmitBuffer.data(), 0, 0);
	d3dContent->PSSetConstantBuffers(0, 1, &_psConstantBuffer);
}

bool DX11ConstantBufferManager::init()
{
	do 
	{
		ID3D11Device* d3dDevice = GraphicMgr::instance()->getD3D11Device();
		ID3D11DeviceContext* d3dContent = GraphicMgr::instance()->getD3D11DeviceContext();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = CONSTANT_SLOT_NUMBER * sizeof(Vector4);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		d3dDevice->CreateBuffer(&bd, NULL, &_vsConstantBuffer);
		d3dDevice->CreateBuffer(&bd, NULL, &_psConstantBuffer);
		SET_DEBUG_NAME(_vsConstantBuffer, "VsConstantBuffer");
		SET_DEBUG_NAME(_psConstantBuffer, "PsConstantBuffer");

		// Set primitive topology
		d3dContent->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return true;
	} while (0);
	return false;
}
