//DECLEAR_SHADER:pass_through SHADER_TYPE:vs ENTRY:main_vs DEFINE:TEST_DEFINE1,_TEST_DEFINE2
#include "common.h"


cbuffer CommonInfo : register(b0)
{

}

PS_INPUT main_vs(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.worldPos = mul(float4(input.pos, 1), g_worldMatrix).xyz;
	output.pos = mul(float4(output.worldPos, 1), g_viewMatrix);
	output.normal = mul(float4(input.normal, 0), g_worldMatrix).xyz;
	output.pos = mul(output.pos, g_projectionMatrix);

	output.tex = input.tex;

	return output;
}