#pragma once
#if defined(TINY_PLATFORM_WINDOWS)
	#ifdef PLATFORM_DURANGO
		#include <d3d11_x.h>
		#include <d3dcompiler_x.h>
	#else
		#include <d3d11.h>
		#include <d3dcompiler.h>
	#endif
#endif




#if defined(TINY_PLATFORM_WINDOWS)
	typedef ID3DBlob					ShaderBlob;
	typedef ID3D11VertexShader			CompiledVSShader;
	typedef ID3D11PixelShader			CompiledPSShader;
	typedef ID3D11Buffer				GpuBuffer;
	typedef	D3D11_INPUT_ELEMENT_DESC	InputLayoutElement;
	typedef ID3D11Buffer				PlatformIndexBuffer;
	typedef ID3D11Buffer				PlatformVertexBuffer;

#if defined(TINY_PLATFORM_WINDOWS)
	#ifndef TINY_RETAIL
		#define SET_DEBUG_NAME(D3DOBJ,NAME) do{D3DOBJ->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)std::strlen(NAME), (NAME));}while(0)
	#else
		#define SET_DEBUG_NAME(D3DOBJ,NAME) do{}while(0)
	#endif
#else
	#error unknown platform
#endif

#elif TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_OPENGL

#else
	#error Unknown Platform
#endif

const static int SHADOW_MAP_WIDTH = 1024;
const static int SHADOW_MAP_HEIGHT = 1024;