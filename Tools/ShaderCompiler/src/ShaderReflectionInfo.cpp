#include "precomp.h"
#include "ShaderReflectionInfo.h"
#include <d3dcompiler.h>
#include "Ash/TinyAssert.h"

ShaderReflectionParser::ShaderReflectionParser(const Path & binaryShader)
	:_binaryShader(binaryShader)
{

}

std::vector<ParamInfo> ShaderReflectionParser::getParamInfo()
{
	std::vector<ParamInfo> ret;
	File file;
	if (!file.open(_binaryShader))
		return ret;

	ID3D11ShaderReflection* pReflector = NULL;
	std::vector<char> data = file.readAll();
	HRESULT result = D3DReflect(data.data(), data.size(), IID_ID3D11ShaderReflection, (void**)&pReflector);
	if (FAILED(result))
		return ret;

	D3D11_SHADER_DESC shaderDesc;
	result = pReflector->GetDesc(&shaderDesc);
	if (FAILED(result))
		return ret;

	ID3D11ShaderReflectionConstantBuffer* constantBuffer = pReflector->GetConstantBufferByName("$Globals");
	D3D11_SHADER_BUFFER_DESC bufferDesc;
	result = constantBuffer->GetDesc(&bufferDesc);
	if (FAILED(result))
		return ret;

	for(UINT i = 0 ; i < bufferDesc.Variables; ++ i)
	{
		ID3D11ShaderReflectionVariable* refVal = constantBuffer->GetVariableByIndex(i);
		D3D11_SHADER_VARIABLE_DESC valDesc;
		result = refVal->GetDesc(&valDesc);
		TinyAssert(result == S_OK);

		ret.emplace_back(valDesc.Name, int(valDesc.StartOffset), int(valDesc.Size));
	}

	
	return ret;
}
