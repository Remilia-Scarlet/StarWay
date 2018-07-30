#pragma once

#include "ShaderConstantInfoDef.h"
#include "Ash/TinyAssert.h"

/***************************************************************************
* This file is auto generated by ShaderCompiler. Don't modify it manually. *
****************************************************************************/

enum class VertexShaders
{
	PASS_THROUGH,               //From: pass_through_vs.hlsl

	SHADER_COUNT
};

enum class PixelShaders
{
	PASS_THROUGH,               //From: pass_through_ps.hlsl
	PASS_THROUGH_NO_TEX,        //From: pass_through_ps.hlsl
	DEFAULT_MATERIAL,           //From: default_material.hlsl
	DEFAULT_MATERIAL_NO_TEX,    //From: default_material.hlsl

	SHADER_COUNT
};

namespace VsLocalConstant
{
	enum class PASS_THROUGH
	{
		WTF1 = 1001,
		WTF2 = 1002,
		WTF3 = 1003,
	};
}

namespace PsLocalConstant
{
	enum class PASS_THROUGH
	{
		AMBIENT = 1001,
		DIFFUSE = 1002,
		SPECULAR = 1003,
		EMIT = 1004,
	};
	enum class PASS_THROUGH_NO_TEX
	{
		AMBIENT = 2001,
		DIFFUSE = 2002,
		SPECULAR = 2003,
		EMIT = 2004,
	};
	enum class DEFAULT_MATERIAL
	{
		AMBIENT = 3001,
		DIFFUSE = 3002,
		SPECULAR = 3003,
		EMIT = 3004,
	};
	enum class DEFAULT_MATERIAL_NO_TEX
	{
		AMBIENT = 4001,
		DIFFUSE = 4002,
		SPECULAR = 4003,
		EMIT = 4004,
	};
}

inline LocalConstantInfo getVsLocalParamInfo(int constant)
{
	switch(constant)
	{
		case 1001:return {0,8};
		case 1002:return {1,64};
		case 1003:return {5,16};
		default:
			TinyAssert(false);
;			return {-1,-1};
	}
}

inline LocalConstantInfo getPsLocalParamInfo(int constant)
{
	switch(constant)
	{
		case 1001:return {0,16};
		case 1002:return {1,16};
		case 1003:return {2,16};
		case 1004:return {3,16};
		case 2001:return {0,16};
		case 2002:return {1,16};
		case 2003:return {2,16};
		case 2004:return {3,16};
		case 3001:return {0,16};
		case 3002:return {1,16};
		case 3003:return {2,16};
		case 3004:return {3,16};
		case 4001:return {0,16};
		case 4002:return {1,16};
		case 4003:return {2,16};
		case 4004:return {3,16};
		default:
			TinyAssert(false);
			return {-1,-1};
	}
}

