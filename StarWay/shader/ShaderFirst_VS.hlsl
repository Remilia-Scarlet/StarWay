#include "common.h"
matrix View : register(c0);
matrix Projection : register(c4);
matrix World : register(c8);
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main(VS_INPUT_COMMON input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.pos = mul( input.pos, World );
    output.pos = mul( output.pos, View );
    output.pos = mul( output.pos, Projection );
    output.tex = input.tex;
    
    return output;
}
