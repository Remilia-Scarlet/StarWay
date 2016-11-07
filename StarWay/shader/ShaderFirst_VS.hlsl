#include "common.h"
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main(VS_INPUT_COMMON input )
{
    PS_INPUT output = (PS_INPUT)0;
	output.worldPos = mul(float4(input.pos, 1), g_worldMatrix).xyz;
    output.pos = mul(float4(output.worldPos,1), g_viewMatrix );
	output.normal = mul(float4(input.normal,0), g_worldMatrix).xyz;
    output.pos = mul( output.pos, g_projectionMatrix );

    output.tex = input.tex;
    
    return output;
}
