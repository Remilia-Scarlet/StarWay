#include "common.h"

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( PS_INPUT input) : SV_Target
{
	float4 color = txDiffuse.Sample(samLinear, input.tex);
	color += CalcLight(input);
    return color;
}
