#include "precomp.h"
#include "GfxShaderVertex.h"
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include <fstream>
#include "Graphic\DX11\DX11Wrapper.h"

GfxShaderVertexPtr GfxShaderVertex::create(const std::string& shaderFileName)
{
	GfxShaderVertex* ret = new GfxShaderVertex();
	if (!ret || !ret->init(shaderFileName))
		TINY_SAFE_DELETE(ret);
	GfxShaderVertexPtr ptr(ret);
	return ptr;
}

GfxShaderVertexPtr GfxShaderVertex::create(CompiledVSShader* shader)
{
	GfxShaderVertex* ret = new GfxShaderVertex();
	if (!ret || !ret->init(shader))
		TINY_SAFE_DELETE(ret);
	GfxShaderVertexPtr ptr(ret);
	return ptr;
}

GfxShaderVertex::GfxShaderVertex()
	: _compiledVSShader(nullptr)
{
}


GfxShaderVertex::~GfxShaderVertex()
{
	TINY_SAFE_RELEASE(_compiledVSShader);
}

bool GfxShaderVertex::createVSShaderFromFile(const std::string& filename)
{
	const std::string fixedFileName = std::string("Shader/") + filename;
	std::ifstream shaderFile(fixedFileName, std::ios::binary);
	if (shaderFile)
	{
		shaderFile.seekg(0, std::ios::end);
		int size = (int)shaderFile.tellg();
		shaderFile.seekg(0, std::ios::beg);
		char* blob = new char[size];
		shaderFile.read(blob, size);
		shaderFile.close();
		CompiledVSShader* vsShader;
		HRESULT hr = GraphicMgr::instance()->getD3D11Device()->CreateVertexShader(blob, (size_t)size, NULL, &vsShader);
		delete[] blob;
		TinyAssert(!FAILED(hr));
		if (FAILED(hr))
			return false;
		_compiledVSShader = vsShader;
		SET_DEBUG_NAME(_compiledVSShader, FormatString("VertexShader_%s", filename.c_str()).c_str());
		return true;
	}
	DebugString("Can't open %s for compiling shader", fixedFileName.c_str());
	return false;
}

void GfxShaderVertex::render()
{
	GraphicMgr::instance()->setVertexShader(GfxShaderVertexPtr(this));
}

bool GfxShaderVertex::init(CompiledVSShader* shader)
{
	do 
	{
		_compiledVSShader = shader;
		return true;
	} while (0);
	return false;
}

bool GfxShaderVertex::init(const std::string& shaderFileName)
{
	do 
	{
		TINY_BREAK_IF(!createVSShaderFromFile(shaderFileName));
		return true;
	} while (0);
	return false;
}
