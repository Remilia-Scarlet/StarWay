#include "common.h"

cbuffer CommonInfo : register(b0)
{
	float3 g_cameraPos : register(c0);
	Material g_material : register(c1);
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( PS_INPUT input) : SV_Target
{
	float4 color = float4(0.6,0.6,0.6,1);
	color = float4(0, 0, 0, 1);
	color += CalcLight(input, g_cameraPos, g_material);
    return color;
}
