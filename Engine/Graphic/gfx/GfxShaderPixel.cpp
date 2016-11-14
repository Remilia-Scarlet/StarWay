#include "Graphic\precomp.h"
#include "GfxShaderPixel.h"
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include <fstream>
#include "Graphic\DX11\DX11Wrapper.h"

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
	const std::string fixedFileName = std::string("Shader/") + filename;
	std::ifstream shaderFile(fixedFileName, std::ios::binary);
	if (shaderFile)
	{
		shaderFile.seekg(0, std::ios::end);
		size_t size = (size_t)shaderFile.tellg();
		shaderFile.seekg(0, std::ios::beg);
		char* blob = new char[size];
		shaderFile.read(blob, size);
		shaderFile.close();
		CompiledPSShader* psShader;
		HRESULT hr = GraphicMgr::instance()->getD3D11Device()->CreatePixelShader(blob, size, NULL, &psShader);
		delete[] blob;
		TinyAssert(!FAILED(hr));
		if (FAILED(hr))
			return false;
		_compiledPSShader = psShader;
		SET_DEBUG_NAME(_compiledPSShader, FormatString("PixelShader_%s", filename.c_str()).c_str());
		return true;
	}
	DebugString("Can't open %s for compiling shader", fixedFileName.c_str());
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
