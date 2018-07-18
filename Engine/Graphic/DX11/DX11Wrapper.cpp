#include "Graphic/precomp.h"
#include "DX11Wrapper.h"


bool DX11Wrapper::CompileShader(const char* shaderCode, int codeLen, const char* entryName,const char* target, ID3DBlob** ppCode)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* blob = NULL;
	hr = D3DCompile(shaderCode,
		codeLen,
		NULL,
		NULL,
		NULL,
		entryName,
		target,
		dwShaderFlags,
		0,
		&blob,
		&pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			DebugString((char*)pErrorBlob->GetBufferPointer());
		TINY_SAFE_RELEASE(pErrorBlob);
		TinyAssert(false);
		return false;
	}
	TINY_SAFE_RELEASE(pErrorBlob);
	*ppCode = blob;
	return true;
}

DX11Wrapper::DX11Wrapper()
{
}


DX11Wrapper::~DX11Wrapper()
{
}
