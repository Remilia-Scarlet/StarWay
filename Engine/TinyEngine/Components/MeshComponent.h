#pragma once
#include "BaseComponent.h"
#include "Graphic\gfx\GfxMesh.h"
#include "Graphic\Manager\ShaderMgr.h"
#include "TinyEngine\ScriptManager\LuaManager.h"
#include "Math\vector\Vector.h"

TINY_DEFINE_PTR(MeshComponent);
class MeshComponent : public BaseComponent
{
public:
	static bool createLuaPrototype();
public:
	virtual ~MeshComponent();
public:
	template<class VertexType, class IndiceType>
	static MeshComponentPtr create(InputLayoutType inputLayout, const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices,const std::string& vsFilename);

	void setMaterial(const Material& material);
	const Material& getMaterial();

	virtual void render() override;

	GfxMeshPtr getGfxMesh() { return _gfxMesh; }
	GfxShaderVertexPtr getVertexShader() { return _vertexShader; }
protected:
	template<class VertexType, class IndiceType>
	bool init(InputLayoutType inputLayout, const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices, const std::string& vsFilename);
	MeshComponent();
	GfxMeshPtr _gfxMesh;
	GfxShaderVertexPtr _vertexShader;
};

template<class VertexType, class IndiceType>
MeshComponentPtr MeshComponent::create(InputLayoutType inputLayout, const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices, const std::string& vsFilename)
{
	MeshComponent* ret = new MeshComponent();
	if (!ret || !ret->init(inputLayout, vertexs, indices,vsFilename))
		TINY_SAFE_DELETE(ret);
	return MeshComponentPtr(ret);
}

template<class VertexType, class IndiceType>
bool MeshComponent::init(InputLayoutType inputLayout,const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices, const std::string& vsFilename)
{
	do
	{
		static_assert(sizeof(IndiceType) == 2 || sizeof(IndiceType) == 4, "You can only use 16 bit or 32 bit type to save index buffer");
		_gfxMesh = GfxMesh::create(inputLayout,
			vertexs.data(), 
			(int)vertexs.size() * sizeof(VertexType),
			sizeof(VertexType), 
			indices.data(), 
			(int)indices.size() * sizeof(IndiceType),
			(sizeof(IndiceType) == 2 ? IndexBufferDataFormat::BIT_16 : IndexBufferDataFormat::BIT_32)
			);
		TINY_BREAK_IF(!_gfxMesh.isValid());
		_vertexShader = ShaderMgr::instance()->getVSShader(vsFilename);
		TINY_BREAK_IF(!_vertexShader.isValid());

		Material defaultMat;
		defaultMat.ambient = { 0.48f,0.77f,0.46f,1.0f };
		defaultMat.diffuse = { 0.48f,0.77f,0.46f,1.0f };
		defaultMat.specular = { 0.2f,0.2f,0.2f,16.0f };
		setMaterial(defaultMat);

		return true;
	} while (0);
	return false;
}

