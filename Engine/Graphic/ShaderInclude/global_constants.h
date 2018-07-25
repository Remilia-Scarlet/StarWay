#ifdef _SHADER
	#define BEGING_GLOBAL_CONSTANT(_NAME_) cbuffer _NAME_ : register(b1){
	#define END_GLOBAL_CONSTANT };
#else
	#define BEGING_GLOBAL_CONSTANT(_NAME_) struct alignas(16) _NAME_{
	#define END_GLOBAL_CONSTANT };
	typedef Matrix4 matrix;
	typedef Vector4 float4;
	typedef Vector3 float3;
	typedef Vector2 float2;
#endif

BEGING_GLOBAL_CONSTANT(VsGlobalConstantBuffer)
	matrix g_viewMatrix;
	matrix g_projectionMatrix;
	matrix g_worldMatrix;
END_GLOBAL_CONSTANT

BEGING_GLOBAL_CONSTANT(PsGlobalConstantBuffer)
	float3 g_CameraPos;
END_GLOBAL_CONSTANT