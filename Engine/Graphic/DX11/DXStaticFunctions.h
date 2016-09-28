#pragma once
class DXStaticFunctions
{
public:
	static bool CompileShader(const char* shaderCode, int codeLen, const char* entryName,const char* target, ID3DBlob** ppCode);

protected:
	DXStaticFunctions();
	~DXStaticFunctions();
};

