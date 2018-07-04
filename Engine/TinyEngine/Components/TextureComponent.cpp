#include "precomp.h"
#include "TextureComponent.h"
#include <fstream>
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\DefaultMgr.h"
#include "Ash/FileSystem/fs_include.h"

TextureComponentPtr TextureComponent::create( const std::string& fileName, const std::string& shadeName)
{
	TextureComponent* ret = new TextureComponent();
	if (!ret || !ret->init(fileName, shadeName))
		TINY_SAFE_DELETE(ret);
	return TextureComponentPtr(ret);
}

TextureComponentPtr TextureComponent::create()
{
	TextureComponent* ret = new TextureComponent();
	if (!ret || !ret->init("",""))
		TINY_SAFE_DELETE(ret);
	return TextureComponentPtr(ret);
}

TextureComponentPtr TextureComponent::create(const std::string & fileName)
{
	TextureComponent* ret = new TextureComponent();
	if (!ret || !ret->init(fileName, ""))
		TINY_SAFE_DELETE(ret);
	return TextureComponentPtr(ret);
}

void TextureComponent::setTexture(const GfxTexturePtr& texture)
{
	_gfxTexture = texture;
}

const GfxTexturePtr& TextureComponent::getTexture()
{
	return _gfxTexture;
}

void TextureComponent::render()
{
	if (_gfxTexture.isValid())
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
		DefaultMgr::instance()->getDefaultPSWithNoTex()->render();
}

void TextureComponent::setMaterial(const GfxMaterialPtr& material)
{
	_gfxMaterial = material;
}

const GfxMaterialPtr& TextureComponent::getMaterial()
{
	return _gfxMaterial;
}

void TextureComponent::setShader(GfxShaderPixelPtr psShader)
{
	_psShader = psShader;
}

const GfxShaderPixelPtr& TextureComponent::getShader()
{
	return _psShader;
}

void TextureComponent::setWireFrame(bool wireFrame)
{
	if (_gfxTexture == nullptr || _gfxTexture == DefaultMgr::instance()->getDefaultTexture())
	{
		_gfxTexture = GfxTexture::create();
	}
	_gfxTexture->setWireFrame(wireFrame);
}

bool TextureComponent::isWireFrame()
{
	if (_gfxTexture == nullptr)
		return DefaultMgr::instance()->getDefaultTexture()->isWireFrame();
	return _gfxTexture->isWireFrame();
}

bool TextureComponent::init(const std::string& fileName, const std::string& shadeName)
{
	do 
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
	TinyAssert(false,"TextureComponent::init failed");
	return false;

}

TextureComponent::TextureComponent()
	:BaseComponent(TO_STRING(TextureComponent))
{
}

TextureComponent::~TextureComponent()
{
}

