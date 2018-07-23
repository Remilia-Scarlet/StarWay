#include "TinyEngine/precomp.h"
#include "MaterialComponent.h"
#include <fstream>
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\DefaultMgr.h"
#include "Ash/FileSystem/fs_include.h"

MaterialComponentPtr MaterialComponent::create( const std::string& fileName, const std::string& shadeName)
{
	MaterialComponent* ret = new MaterialComponent();
	if (!ret || !ret->init(fileName, shadeName))
		TINY_SAFE_DELETE(ret);
	return MaterialComponentPtr(ret);
}

MaterialComponentPtr MaterialComponent::create()
{
	MaterialComponent* ret = new MaterialComponent();
	if (!ret || !ret->init("",""))
		TINY_SAFE_DELETE(ret);
	return MaterialComponentPtr(ret);
}

MaterialComponentPtr MaterialComponent::create(const std::string & fileName)
{
	MaterialComponent* ret = new MaterialComponent();
	if (!ret || !ret->init(fileName, ""))
		TINY_SAFE_DELETE(ret);
	return MaterialComponentPtr(ret);
}

void MaterialComponent::setTexture(const GfxTexturePtr& texture)
{
	//_gfxTexture = texture;
}

const GfxTexturePtr& MaterialComponent::getTexture()
{
	//return _gfxTexture;
	return {};
}

void MaterialComponent::render()
{
	/*if (_gfxTexture.isValid())
		_gfxTexture->render();
	else
		DefaultMgr::instance()->getDefaultTexture()->render();

	if (_gfxMaterial.isValid())
		_gfxMaterial->render();
	else
		DefaultMgr::instance()->getDefaultMaterial()->render();

	if (_psShader.isValid())
		_psShader->render();
	else if (_gfxTexture.isValid())
		DefaultMgr::instance()->getDefaultPSWithTex()->render();
	else
		DefaultMgr::instance()->getDefaultPSWithNoTex()->render();*/
}

void MaterialComponent::setMaterial(const MaterialPtr& material)
{
	_gfxMaterial = material;
}

const MaterialPtr& MaterialComponent::getMaterial()
{
	return _gfxMaterial;
}

void MaterialComponent::setShader(GfxShaderPixelPtr psShader)
{
	//_psShader = psShader;
}

const GfxShaderPixelPtr& MaterialComponent::getShader()
{
	//return _psShader;
	return {};
}

void MaterialComponent::setWireFrame(bool wireFrame)
{
	//if (_gfxTexture == nullptr || _gfxTexture == DefaultMgr::instance()->getDefaultTexture())
	//{
	//	_gfxTexture = GfxTexture::create();
	//}
	//_gfxTexture->setWireFrame(wireFrame);
}

bool MaterialComponent::isWireFrame()
{
	//if (_gfxTexture == nullptr)
	//	return DefaultMgr::instance()->getDefaultTexture()->isWireFrame();
	//return _gfxTexture->isWireFrame();
	return false;
}

bool MaterialComponent::init(const std::string& fileName, const std::string& shadeName)
{
	/*do 
	{
		if (!fileName.empty())
		{
			File picFile;
			picFile.open(fileName, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
			std::vector<char> data = picFile.readAll();
			_gfxTexture = GfxTexture::create(reinterpret_cast<uint8_t*>(data.data()), int(data.size()), fileName.c_str());
			TINY_BREAK_IF(!_gfxTexture.isValid());
		}
		else
			_gfxTexture = nullptr;

		GfxMaterialPtr defaultMat = DefaultMgr::instance()->getDefaultMaterial();
		setMaterial(defaultMat);

		_psShader = ShaderMgr::instance()->getPSShader(shadeName);
		return true;
	} while (0);
	TinyAssert(false,"MaterialComponent::init failed");*/
	return false;

}

MaterialComponent::MaterialComponent()
	:BaseComponent(TO_STRING(MaterialComponent))
{
}

MaterialComponent::~MaterialComponent()
{
}

