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
};

struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
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
	ambient = float4(1, 0, 0, 1);
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

	float att = 1.0f / dot(L.att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}

void ComputeDirectionalLight(Material mat, 
	DirectionalLight L,
	float3 normal, 
	float3 toEye,
	out float4 ambient, 
	out float4 diffuse, 
	out float4 spec) 
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -L.direction;

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
}

float4 CalcLight(PS_INPUT input, float3 cameraPos, Material material)
{
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float directLightNum = g_lightNumber.x;
	float pointLightNum = g_lightNumber.y;
	float spotLightNum = g_lightNumber.z;
	if (directLightNum != 0 || pointLightNum != 0 || spotLightNum != 0)
	{
		input.normal = normalize(input.normal);
		float3 toEyeW = normalize(cameraPos - input.worldPos);
		float4 A, D, S;
		for (int i = 0; i < directLightNum; ++i)
		{
			ComputeDirectionalLight(material, g_directionalLight[i], input.normal, toEyeW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}

		for (int j = 0; j < pointLightNum; ++j)
		{
			ComputePointLight(material, g_pointLight[j], float4(input.worldPos ,1), input.normal, toEyeW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}

		for (int k = 0; k < spotLightNum; ++k)
		{
			ComputeSpotLight(material, g_spotLight[k], float4(input.worldPos,1), input.normal, toEyeW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}
	}
	float4 litColor = ambient + diffuse + spec;
	litColor.a = material.diffuse.a;
	return litColor;
}