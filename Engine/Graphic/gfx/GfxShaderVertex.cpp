#include "Graphic/precomp.h"
#include "GfxShaderVertex.h"
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include <fstream>
#include "Ash/FileSystem/fs_include.h"

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
	File shaderFile;
	shaderFile.open(filename, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	if (shaderFile.isOpened())
	{
		std::vector<char> data = shaderFile.readAll();
		CompiledVSShader* vsShader;
		HRESULT hr = GraphicMgr::instance()->getD3D11Device()->CreateVertexShader(data.data(), data.size(), NULL, &vsShader);
		TinyAssert(!FAILED(hr));
		if (FAILED(hr))
			return false;
		_compiledVSShader = vsShader;
		SET_DEBUG_NAME(_compiledVSShader, FormatString("VertexShader_%s", filename.c_str()).c_str());
		return true;
	}
	DebugString("Can't open %s for compiling shader", shaderFile.getFilePath().getAbsolutePath().c_str());
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
