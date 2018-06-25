/*
DECLEAR_SHADER:pass_through SHADER_TYPE:ps ENTRY:main_ps DEFINE:
DECLEAR_SHADER:pass_through_no_tex SHADER_TYPE:ps ENTRY:main_ps DEFINE:SAMPLE_TEX
*/
#include "common.h"

cbuffer CommonInfo : register(b0)
{
	float3 g_cameraPos : register(c0);
	Material g_material : register(c1);
}

#ifdef SAMPLE_TEX
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);
#endif



float4 main_ps(PS_INPUT input, float4 pos : SV_POSITION) : SV_Target
{
#ifdef SAMPLE_TEX
	float4 color = txDiffuse.Sample(samLinear, input.tex);
#else
	float4 color = float4(0, 0, 0, 0);
#endif
	color = CalcLight(input, g_cameraPos, g_material, color);
	return color;
}
