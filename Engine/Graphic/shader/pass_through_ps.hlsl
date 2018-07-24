/*
DECLEAR_SHADER:pass_through SHADER_TYPE:ps ENTRY:main_ps DEFINE:
DECLEAR_SHADER:pass_through_no_tex SHADER_TYPE:ps ENTRY:main_ps DEFINE:SAMPLE_TEX
*/
#include "common.h"
#include "global_constants.h"

#ifdef SAMPLE_TEX
DECLARE_TEXTURE2D(diffuse_tex, 0);
#endif

BEGING_LOCAL_CONSTANT
	DECLEAR_LOCAL_CONSTANT(float4, ambient, 0);
	DECLEAR_LOCAL_CONSTANT(float4, diffuse, 1);
	DECLEAR_LOCAL_CONSTANT(float3, specular, 2.xyz);
	DECLEAR_LOCAL_CONSTANT(float,  weight, 2.w);
	DECLEAR_LOCAL_CONSTANT(float4, emit, 3);
END_LOCAL_CONSTANT

float4 main_ps(PS_INPUT input, float4 pos : SV_POSITION) : SV_Target
{
#ifdef SAMPLE_TEX
	float4 color = tex2D(diffuse_tex, input.tex);
#else
	float4 color = float4(0, 0, 0, 0);
#endif
	color = CalcLight(input, g_CameraPos, g_Material, color);
	return color;
}
