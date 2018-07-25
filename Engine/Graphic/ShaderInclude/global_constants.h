#ifdef _SHADER
	#define BEGING_GLOBAL_CONSTANT(_NAME_) cbuffer _NAME_ : register(b1){
	#define DECLEAR_GLOBAL_CONSTANT(_TYPE_,_NAME_) _TYPE_ g_##_NAME_;
	#define END_GLOBAL_CONSTANT };
#else
	#define BEGING_GLOBAL_CONSTANT(_NAME_) struct alignas(16) _NAME_{
	#define DECLEAR_GLOBAL_CONSTANT(_TYPE_,_NAME_) private: _TYPE_ _##_NAME_; public: const _TYPE_& get##_NAME_(){return _##_NAME_;} void set##_NAME_(const _TYPE_& val){ _##_NAME_ = val;}
	#define END_GLOBAL_CONSTANT };
	typedef Matrix4 matrix;
	typedef Vector4 float4;
	typedef Vector3 float3;
	typedef Vector2 float2;
#endif

BEGING_GLOBAL_CONSTANT(VsGlobalConstantBuffer)
	DECLEAR_GLOBAL_CONSTANT(matrix,ViewMatrix)
	DECLEAR_GLOBAL_CONSTANT(matrix,ProjectionMatrix)
	DECLEAR_GLOBAL_CONSTANT(matrix,WorldMatrix)
END_GLOBAL_CONSTANT

BEGING_GLOBAL_CONSTANT(PsGlobalConstantBuffer)
	DECLEAR_GLOBAL_CONSTANT(float3,CameraPos)
END_GLOBAL_CONSTANT