#pragma once
#include "BaseComponent.h"
#include "Graphic/gfx/GfxTexture.h"
#include "Graphic/DX11/DX11GraphicMgr.h"
#include "DangoEngine/Graphic/Material.h"

ASH_DEFINE_PTR(MaterialComponent);
class MaterialComponent : public BaseComponent
{
public:
	virtual ~MaterialComponent();
public:
	static MaterialComponentPtr create();
	static MaterialComponentPtr create(const std::string& fileName);

	void setMaterial(const MaterialPtr& material);
	const MaterialPtr& getMaterial() const;

	void setWireFrame(bool wireFrame);
	bool isWireFrame();

	virtual void render() override;
protected:
	virtual bool init(const std::string& fileName);
	MaterialComponent();

	MaterialPtr _material;
};

