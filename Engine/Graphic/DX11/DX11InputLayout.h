#pragma once
#include "Graphic/gfx/GfxInputLayout.h"


TINY_DEFINE_PTR(DX11InputLayout);
class DX11InputLayout : public GfxInputLayout
{
	friend class GfxInputLayout;
	friend class DX11GraphicMgr;
public:
	virtual void render() override;
protected:
	DX11InputLayout();
	virtual ~DX11InputLayout();
	static DX11InputLayoutPtr create(ID3D11InputLayout* d3dInputLayout);
	virtual bool init(ID3D11InputLayout* d3dInputLayout);

	ID3D11InputLayout* _d3dInputLayout;
};

