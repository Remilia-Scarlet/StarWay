//DECLEAR_SHADER:pass_through SHADER_TYPE:vs ENTRY:main_vs DEFINE:TEST_DEFINE1,_TEST_DEFINE2
#include "common.h"
#include "global_constants.h"


DECLEAR_LOCAL_CONSTANT(float2, wtf1, 0);
DECLEAR_LOCAL_CONSTANT(matrix, wtf2, 1);
DECLEAR_LOCAL_CONSTANT(float4, wtf3, 5);

PS_INPUT main_vs(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.worldPos = mul(float4(input.pos, 1), g_WorldMatrix).xyz;
	output.worldPos.xy = wtf1;
	output.pos = mul(float4(output.worldPos, 1), g_ViewMatrix);
	output.normal = mul(float4(input.normal, 0), g_WorldMatrix).xyz;
	output.pos = mul(output.pos, g_ProjectionMatrix);

	output.tex = input.tex;

	return output;
}