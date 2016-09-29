#include "common.h"

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );
float4 vMeshColor : register(c0);

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( PS_INPUT input) : SV_Target
{
    return txDiffuse.Sample( samLinear, input.tex ) *1 /*vMeshColor*/;
}
