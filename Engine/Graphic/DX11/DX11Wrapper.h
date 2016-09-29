#pragma once
class DX11Wrapper
{
public:
	static bool CompileShader(const char* shaderCode, int codeLen, const char* entryName,const char* target, ID3DBlob** ppCode);

protected:
	DX11Wrapper();
	~DX11Wrapper();
};

