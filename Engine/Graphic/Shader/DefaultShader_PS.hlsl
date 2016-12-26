#include "common.h"

cbuffer CommonInfo : register(b0)
{
	float3 g_cameraPos : register(c0);
	Material g_material : register(c1);
}

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( PS_INPUT input, float4 pos : SV_POSITION) : SV_Target
{
	float4 color = txDiffuse.Sample(samLinear, input.tex);
	color = CalcLight(input, g_cameraPos, g_material, color);
    return color;
}
