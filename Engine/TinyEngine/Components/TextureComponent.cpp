#include "TinyEngine\precomp.h"
#include "TextureComponent.h"
#include <fstream>
#include "Graphic\Manager\ShaderMgr.h"

TextureComponentPtr TextureComponent::create( const std::string& fileName, const std::string& shadeName)
{
	TextureComponent* ret = new TextureComponent();
	if (!ret || !ret->init(fileName, shadeName))
		TINY_SAFE_DELETE(ret);
	return TextureComponentPtr(ret);
}

void TextureComponent::render()
{
	_gfxTexture->render();
	_gfxMaterial->render();
	_psShader->render();
}

void TextureComponent::setMaterial(const GfxMaterialPtr& material)
{
	_gfxMaterial = material;
}

const GfxMaterialPtr& TextureComponent::getMaterial()
{
	return _gfxMaterial;
}

bool TextureComponent::init(const std::string& fileName, const std::string& shadeName)
{
	do 
	{
		if (!fileName.empty())
		{
			std::ifstream file(fileName, std::ios::binary);
			TINY_BREAK_IF(!file);
			file.seekg(0, std::ios::end);
			size_t size = (size_t)file.tellg();
			file.seekg(0, std::ios::beg);
			char* buffer = new char[size];
			file.read(buffer, size);
			file.close();
			_gfxTexture = GfxTexture::create((uint8_t*)buffer, (int)size, fileName.c_str());
			delete[] buffer;
		}
		else
			_gfxTexture = GfxTexture::create(nullptr, 0, fileName.c_str());

		TINY_BREAK_IF(!_gfxTexture.isValid());

		GfxMaterialPtr defaultMat = GfxMaterial::create({ 0.48f, 0.77f, 0.46f, 1.0f }, { 0.48f, 0.77f, 0.46f, 1.0f }, { 0.2f, 0.2f, 0.2f, 16.0f });
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


bool TextureComponent::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);

	LUA_PROTOTYPE_END(TextureComponent);
	return true;
}

TextureComponent::~TextureComponent()
{
}

