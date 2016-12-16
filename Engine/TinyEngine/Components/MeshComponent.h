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
	template<class VertexType>
	static MeshComponentPtr create(InputLayoutType inputLayout, const std::vector<VertexType>& vertexs, const std::string& vsFilename);
	static int L_create(lua_State* L);

	void setMaterial(const GfxMaterialPtr& material);
	const GfxMaterialPtr& getMaterial();

	void setPrimitiveTopology(PrimitiveTopology primitiveTopology);
	static int L_setPrimitiveTopology(lua_State* L);

	PrimitiveTopology getPrimitiveTopology() const;


	virtual void render() override;

	GfxMeshPtr getGfxMesh() { return _gfxMesh; }
	GfxShaderVertexPtr getVertexShader() { return _vertexShader; }
protected:
	template<class VertexType, class IndiceType>
	bool init(InputLayoutType inputLayout, const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices, const std::string& vsFilename);
	template<class VertexType>
	bool init(InputLayoutType inputLayout, const std::vector<VertexType>& vertexs, const std::string& vsFilename);
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

template<class VertexType>
MeshComponentPtr MeshComponent::create(InputLayoutType inputLayout, const std::vector<VertexType>& vertexs, const std::string & vsFilename)
{
	MeshComponent* ret = new MeshComponent();
	if (!ret || !ret->init(inputLayout, vertexs, vsFilename))
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

		GfxMaterialPtr defaultMat =	GfxMaterial::create({0.48f, 0.77f, 0.46f, 1.0f}, {0.48f, 0.77f, 0.46f, 1.0f}, {0.2f, 0.2f, 0.2f, 16.0f}, {0.f, 0.f, 0.f, 0.f});
		setMaterial(defaultMat);

		return true;
	} while (0);
	return false;
}

template<class VertexType>
bool MeshComponent::init(InputLayoutType inputLayout, const std::vector<VertexType>& vertexs, const std::string& vsFilename)
{
	if (vertexs.size() > (size_t)(uint16_t)-1)
	{
		std::vector<uint32_t> indices(vertexs.size());
		for (int i = 0; i < (int)vertexs.size(); ++i)
			indices[i] = i;
		return init(inputLayout, vertexs, indices, vsFilename);
	}
	else
	{
		std::vector<uint16_t> indices(vertexs.size());
		for (int i = 0; i < (int)vertexs.size(); ++i)
			indices[i] = i;
		return init(inputLayout, vertexs, indices, vsFilename);
	}
}

