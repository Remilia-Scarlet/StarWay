#include "precomp.h"
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

	for (auto& pair : _registVSBufferMap)
		TINY_SAFE_RELEASE(pair.second);

	for (auto& pair : _registPSBufferMap)
		TINY_SAFE_RELEASE(pair.second);
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

void DX11ConstantBufferManager::registVSBuffer(int bufferSlot, int size)
{
	TinyAssert(bufferSlot > 0);
	if (bufferSlot < 0)
		return;
	auto it = _registVSBufferMap.find(bufferSlot);
	if (it != _registVSBufferMap.end())
	{
		TinyAssert(false, "already registed");
		return;
	}
	ID3D11Buffer* buffer = nullptr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	GraphicMgr::instance()->getD3D11Device()->CreateBuffer(&bd, NULL, &buffer);
	SET_DEBUG_NAME(buffer, "registed buffer");
	_registVSBufferMap[bufferSlot] = buffer;
}

void DX11ConstantBufferManager::registPSBuffer(int bufferSlot, int size)
{
	TinyAssert(bufferSlot > 0);
	if (bufferSlot < 0)
		return;
	auto it = _registPSBufferMap.find(bufferSlot);
	if (it != _registPSBufferMap.end())
	{
		TinyAssert(false, "already registed");
		return;
	}
	ID3D11Buffer* buffer = nullptr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	GraphicMgr::instance()->getD3D11Device()->CreateBuffer(&bd, NULL, &buffer);
	if (!buffer)
	{
		TinyAssert(false);
		return;
	}
	SET_DEBUG_NAME(buffer, "registed buffer");
	_registPSBufferMap[bufferSlot] = buffer;
}

void DX11ConstantBufferManager::setRegistVSBuffer(int bufferSlot, void* data)
{
	auto it = _registVSBufferMap.find(bufferSlot);
	if (it == _registVSBufferMap.end())
	{
		TinyAssert(false, "not found");
		return;
	}
	ID3D11DeviceContext* d3dContent = GraphicMgr::instance()->getD3D11DeviceContext();
	ID3D11Buffer* buffer = it->second;
	d3dContent->UpdateSubresource(buffer, 0, NULL, data, 0, 0);
	d3dContent->VSSetConstantBuffers(bufferSlot, 1, &buffer);
}

void DX11ConstantBufferManager::setRegistPSBuffer(int bufferSlot, void* data)
{
	auto it = _registPSBufferMap.find(bufferSlot);
	if (it == _registPSBufferMap.end())
	{
		TinyAssert(false, "not found");
		return;
	}
	ID3D11DeviceContext* d3dContent = GraphicMgr::instance()->getD3D11DeviceContext();
	ID3D11Buffer* buffer = it->second;
	d3dContent->UpdateSubresource(buffer, 0, NULL, data, 0, 0);
	d3dContent->PSSetConstantBuffers(bufferSlot, 1, &buffer);
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
		return true;
	} while (0);
	return false;
}
