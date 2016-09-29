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
	_psShader->render();
}

bool TextureComponent::init(const std::string& fileName, const std::string& shadeName)
{
	do 
	{
		std::ifstream file(fileName,std::ios::binary);
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

		_psShader = ShaderMgr::instance()->getPSShader(shadeName);
		return true;
	} while (0);
	TinyAssert(false,"TextureComponent::init failed");
	return false;

}

TextureComponent::TextureComponent()
	:BaseComponent()
{
}


TextureComponent::~TextureComponent()
{
}

