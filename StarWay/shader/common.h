struct VS_INPUT_DEFAULT
{
	float4 pos : POSITION;
};

struct VS_INPUT_COMMON
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};
