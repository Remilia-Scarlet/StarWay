struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 worldPos : POSITION; // world position
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;// w is weight
	float4 emit;
};

struct DirectionalLight
{
	float4 ambient;//xyz:color w:intensity
	float4 diffuse;//xyz:color w:intensity
	float4 specular;//xyz:color w:intensity
	float3 direction;
	float pad;
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 att;
	float pad;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 direction;
	float spot;

	float3 att;
	float pad;
};

cbuffer LightInfo : register(b1)
{
	float4 g_lightNumber;//x : direction light number; y : point light number; z : spot light number; w : not used
	DirectionalLight g_directionalLight[5];
	PointLight g_pointLight[5];
	SpotLight g_spotLight[5];
};

void ComputeSpotLight(Material mat, 
	SpotLight L, 
	float4 position, 
	float3 normal,  
	float3 toEye,    
	float4 texColor,
	out float4 ambient, 
	out float4 diffuse, 
	out float4 spec) 
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 pos = position.xyz;

	float3 lightVec = L.position - pos;

	float d = length(lightVec);

	if (d > L.range)
		return;

	lightVec /= d;

	ambient = mat.ambient * L.ambient;


	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

		diffuse = diffuseFactor * mat.diffuse * L.diffuse;

		spec = specFactor * mat.specular * L.specular;
	}

	float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.spot);

	float att = spot / dot(L.att, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;

}

void ComputePointLight(Material mat, 
	PointLight L,  
	float4 position,  
	float3 normal, 
	float3 toEye, 
	float4 texColor,
	out float4 ambient, 
	out float4 diffuse, 
	out float4 spec)  
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 pos = position.xyz;

	float3 lightVec = L.position - pos;

	float d = length(lightVec);
	if (d > L.range)
		return;

	lightVec /= d;

	ambient = mat.ambient * L.ambient * L.ambient.w;

	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}

	float att = 1.0f / dot(L.att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}

void ComputeDirectionalLight(Material mat, 
	DirectionalLight L,
	float3 normal, 
	float3 toEye,
	float4 texColor,
	out float4 ambient, 
	out float4 diffuse, 
	out float4 spec) 
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -L.direction;

	ambient = mat.ambient * L.ambient * L.ambient.w;

	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{

		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
		diffuse = diffuseFactor * mat.diffuse * L.diffuse * L.diffuse.w;
		spec = specFactor * mat.specular * L.specular;
	}
}

float4 CalcLight(PS_INPUT input, float3 cameraPos, Material material, float4 texColor)
{
	float directLightNum = g_lightNumber.x;
	float pointLightNum = g_lightNumber.y;
	float spotLightNum = g_lightNumber.z;
	float4 litColor = float4(0, 0, 0, 0);
	if (directLightNum != 0 || pointLightNum != 0 || spotLightNum != 0)
	{
		input.normal = normalize(input.normal);
		float3 toEyeW = normalize(cameraPos - input.worldPos);

		float3 amb = material.ambient.xyz * material.ambient.w;
		float3 dif = material.diffuse.xyz * material.diffuse.w;

		amb *= texColor.xyz * texColor.w + dif * (1 - texColor.w);
		dif = texColor.xyz * texColor.w + dif * (1 - texColor.w);

		material.ambient = float4(amb, 1);
		material.diffuse = float4(dif, 1);

		float4 outA = float4(0, 0, 0, 0);
		float4 outD = float4(0, 0, 0, 0);
		float4 outS = float4(0, 0, 0, 0);
		float4 ambient = float4(0, 0, 0, 0);
		float4 diffuse = float4(0, 0, 0, 0);
		float4 spec = float4(0, 0, 0, 0);
		for (int i = 0; i < directLightNum; ++i)
		{
			ComputeDirectionalLight(material, g_directionalLight[i], input.normal, toEyeW, texColor, outA, outD, outS);
			ambient += outA;
			diffuse += outD;
			spec += outS;
		}

		for (int j = 0; j < pointLightNum; ++j)
		{
			ComputePointLight(material, g_pointLight[j], float4(input.worldPos ,1), input.normal, toEyeW, texColor, outA, outD, outS);
			ambient += outA;
			diffuse += outD;
			spec += outS;
		}

		for (int k = 0; k < spotLightNum; ++k)
		{
			ComputeSpotLight(material, g_spotLight[k], float4(input.worldPos,1), input.normal, toEyeW, texColor, outA, outD, outS);
			ambient += outA;
			diffuse += outD;
			spec += outS;
		}
		litColor = ambient + diffuse + spec;
	}
	litColor.a = 1;
	return litColor;
}