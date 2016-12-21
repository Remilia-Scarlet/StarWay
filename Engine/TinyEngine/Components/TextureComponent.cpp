#include "TinyEngine\precomp.h"
#include "TextureComponent.h"
#include <fstream>
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\DefaultMgr.h"

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

int TextureComponent::L_create(lua_State* L)
{
	int top = lua_gettop(L);
	if (top < 1 || top > 3)
		return LUA_PARAM_ERROR(TextureComponent::L_create);

	if (top == 1)
		LuaManager::instance()->pushVal(L,TextureComponent::create());
	else if (top == 2)
	{
		const char* texName = LuaManager::instance()->getVal<const char*>(L, 2);
		LuaManager::instance()->pushVal(L, TextureComponent::create(texName));
	}
	else if (top == 3)
	{
		const char* texName = LuaManager::instance()->getVal<const char*>(L, 2);
		const char* shaderName = LuaManager::instance()->getVal<const char*>(L, 3);
		LuaManager::instance()->pushVal(L, TextureComponent::create(texName, shaderName));
	}
	return 1;
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

