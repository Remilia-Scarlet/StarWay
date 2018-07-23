#pragma once
#include "BaseComponent.h"
#include "Graphic/gfx/GfxTexture.h"
#include "Graphic/DX11/DX11GraphicMgr.h"
#include "TinyEngine/Graphic/Material.h"

TINY_DEFINE_PTR(MaterialComponent);
class MaterialComponent : public BaseComponent
{
public:
	virtual ~MaterialComponent();
public:
	const static MaterialComponentPtr DefaultTexture;

	static MaterialComponentPtr create();
	static MaterialComponentPtr create(const std::string& fileName);
	static MaterialComponentPtr create(const std::string& fileName, const std::string& shadeName);

	void setTexture(const GfxTexturePtr& texture);
	const GfxTexturePtr& getTexture();

	void setMaterial(const MaterialPtr& material);
	const MaterialPtr& getMaterial();

	void setShader(GfxShaderPixelPtr psShader);
	const GfxShaderPixelPtr& getShader();

	void setWireFrame(bool wireFrame);
	bool isWireFrame();

	virtual void render() override;
protected:
	virtual bool init(const std::string& fileName, const std::string& shadeName);
	MaterialComponent();

	MaterialPtr _gfxMaterial;
};

