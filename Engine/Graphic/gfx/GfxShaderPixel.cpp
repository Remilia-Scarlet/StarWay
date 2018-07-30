#include "Graphic/precomp.h"
#include "GfxShaderPixel.h"
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include <fstream>
#include "Ash/FileSystem/fs_include.h"

GfxShaderPixelPtr GfxShaderPixel::create(const std::string& shaderFileName)
{
	GfxShaderPixel* ret = new GfxShaderPixel();
	if (!ret || !ret->init(shaderFileName))
		TINY_SAFE_DELETE(ret);
	GfxShaderPixelPtr ptr(ret);
	return ptr;
}

GfxShaderPixelPtr GfxShaderPixel::create(ShaderBlob* blob, CompiledPSShader* shader)
{
	GfxShaderPixel* ret = new GfxShaderPixel();
	if (!ret || !ret->init(blob, shader))
		TINY_SAFE_DELETE(ret);
	GfxShaderPixelPtr ptr(ret);
	return ptr;
}

GfxShaderPixel::GfxShaderPixel()
	:_compiledPSShader(nullptr)
{
}


GfxShaderPixel::~GfxShaderPixel()
{
	TINY_SAFE_RELEASE(_compiledPSShader);
}

bool GfxShaderPixel::createPSShaderFromFile(const std::string& filename)
{
	File shaderFile;
	shaderFile.open(filename, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	if (shaderFile.isOpened())
	{
		std::vector<char> data = shaderFile.readAll();
		CompiledPSShader* psShader;
		HRESULT hr = GraphicMgr::instance()->getD3D11Device()->CreatePixelShader(data.data(), data.size(), NULL, &psShader);
		TinyAssert(!FAILED(hr));
		if (FAILED(hr))
			return false;
		_compiledPSShader = psShader;
		SET_DEBUG_NAME(_compiledPSShader, FormatString("PixelShader_%s", filename.c_str()).c_str());
		return true;
	}
	DebugString("Can't open %s for compiling shader", shaderFile.getFilePath().getAbsolutePath().c_str());
	return false;
	
}

void GfxShaderPixel::render()
{
	GraphicMgr::instance()->setPixelShader(GfxShaderPixelPtr(this));
}

bool GfxShaderPixel::init(ShaderBlob* blob, CompiledPSShader* shader)
{
	do
	{
		_compiledPSShader = shader;
		return true;
	} while (0);
	return false;
}

bool GfxShaderPixel::init(const std::string& shaderFileName)
{
	do
	{
		TINY_BREAK_IF(createPSShaderFromFile(shaderFileName) == false);
		return true;
	} while (0);
	return false;
}
