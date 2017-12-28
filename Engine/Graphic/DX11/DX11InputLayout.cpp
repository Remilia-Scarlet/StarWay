#include "precomp.h"
#include "DX11InputLayout.h"
#include "Graphic/DX11/DX11GraphicMgr.h"

void DX11InputLayout::render()
{
	GraphicMgr::instance()->getD3D11DeviceContext()->IASetInputLayout(_d3dInputLayout);
}

bool DX11InputLayout::init(ID3D11InputLayout* d3dInputLayout)
{
	_d3dInputLayout = d3dInputLayout;
	return true;
}

DX11InputLayout::DX11InputLayout()
	:_d3dInputLayout(nullptr)
{

}

DX11InputLayout::~DX11InputLayout()
{
	if (_d3dInputLayout)
		TINY_SAFE_RELEASE(_d3dInputLayout);
}

DX11InputLayoutPtr DX11InputLayout::create(ID3D11InputLayout* d3dInputLayout)
{
	DX11InputLayout* ret = new DX11InputLayout();
	if (ret && ret->init(d3dInputLayout))
		return DX11InputLayoutPtr(ret);
	TINY_SAFE_DELETE(ret);
	return nullptr;
}
