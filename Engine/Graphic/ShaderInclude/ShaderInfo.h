#pragma once

#include "ShaderConstantInfoDef.h"
#include "Ash/TinyAssert.h"

/***************************************************************************
* This file is auto generated by ShaderCompiler. Don't modify it manually. *
****************************************************************************/

enum class VertexShader
{
	PASS_THROUGH,               //From: pass_through_vs.hlsl

	SHADER_COUNT
};

enum class PixelShader
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
	inline int getConstantIdFromName(std::string shaderName, std::string constantName)
	{
		shaderName = toUpper(shaderName);
		constantName = toUpper(constantName);
		if(shaderName == "PASS_THROUGH")
		{
			if (constantName == "WTF1") return 1001;
			else if (constantName == "WTF2") return 1002;
			else if (constantName == "WTF3") return 1003;
		}
		TinyAssert(false, "Not found local constant name");
		return -1;	}
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
	inline int getConstantIdFromName(std::string shaderName, std::string constantName)
	{
		shaderName = toUpper(shaderName);
		constantName = toUpper(constantName);
		if(shaderName == "PASS_THROUGH")
		{
			if (constantName == "AMBIENT") return 1001;
			else if (constantName == "DIFFUSE") return 1002;
			else if (constantName == "SPECULAR") return 1003;
			else if (constantName == "EMIT") return 1004;
		}
		else if(shaderName == "PASS_THROUGH_NO_TEX")
		{
			if (constantName == "AMBIENT") return 2001;
			else if (constantName == "DIFFUSE") return 2002;
			else if (constantName == "SPECULAR") return 2003;
			else if (constantName == "EMIT") return 2004;
		}
		else if(shaderName == "DEFAULT_MATERIAL")
		{
			if (constantName == "AMBIENT") return 3001;
			else if (constantName == "DIFFUSE") return 3002;
			else if (constantName == "SPECULAR") return 3003;
			else if (constantName == "EMIT") return 3004;
		}
		else if(shaderName == "DEFAULT_MATERIAL_NO_TEX")
		{
			if (constantName == "AMBIENT") return 4001;
			else if (constantName == "DIFFUSE") return 4002;
			else if (constantName == "SPECULAR") return 4003;
			else if (constantName == "EMIT") return 4004;
		}
		TinyAssert(false, "Not found local constant name");
		return -1;	}
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
;			return {-1,-1};
	}
}

inline const char* getVertexShaderName(VertexShader shader)
{
	switch(shader)
	{
		case VertexShader::PASS_THROUGH: return "PASS_THROUGH";
		default:
			TinyAssert(false);
			return nullptr;
	}
}

inline const char* getPixelShaderName(PixelShader shader)
{
	switch(shader)
	{
		case PixelShader::PASS_THROUGH: return "PASS_THROUGH";
		case PixelShader::PASS_THROUGH_NO_TEX: return "PASS_THROUGH_NO_TEX";
		case PixelShader::DEFAULT_MATERIAL: return "DEFAULT_MATERIAL";
		case PixelShader::DEFAULT_MATERIAL_NO_TEX: return "DEFAULT_MATERIAL_NO_TEX";
		default:
			TinyAssert(false);
			return nullptr;
	}
}

inline VertexShader getVertexShaderFromStr(std::string name)
{
	name = toUpper(name);
	if (name == "PASS_THROUGH")return VertexShader::PASS_THROUGH;
	TinyAssert(false, "Get shader from name string failed. No matching shader.");
	return static_cast<VertexShader>(-1);
}

inline PixelShader getPixelShaderFromStr(std::string name)
{
	name = toUpper(name);
	if (name == "PASS_THROUGH")return PixelShader::PASS_THROUGH;
	if (name == "PASS_THROUGH_NO_TEX")return PixelShader::PASS_THROUGH_NO_TEX;
	if (name == "DEFAULT_MATERIAL")return PixelShader::DEFAULT_MATERIAL;
	if (name == "DEFAULT_MATERIAL_NO_TEX")return PixelShader::DEFAULT_MATERIAL_NO_TEX;
	TinyAssert(false, "Get shader from name string failed. No matching shader.");
	return static_cast<PixelShader>(-1);
}

