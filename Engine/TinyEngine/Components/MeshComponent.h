#pragma once
#include "BaseComponent.h"
#include "Graphic\gfx\GfxMesh.h"
#include "Graphic\Manager\ShaderMgr.h"
#include "Math\vector\Vector.h"
#include "Graphic\Manager\GraphicMgr.h"

TINY_DEFINE_PTR(MeshComponent);
class MeshComponent : public BaseComponent
{
public:
	virtual ~MeshComponent();
public:
	template<class VertexType, class IndiceType>
	static MeshComponentPtr create(const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices,const std::string& vsFilename);
	template<class VertexType, class IndiceType>
	static MeshComponentPtr create(const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices);
	template<class VertexType>
	static MeshComponentPtr create(const std::vector<VertexType>& vertexs, const std::string& vsFilename);
	template<class VertexType>
	static MeshComponentPtr create(const std::vector<VertexType>& vertexs);

	void setPrimitiveTopology(PrimitiveTopology primitiveTopology);
	PrimitiveTopology getPrimitiveTopology() const;


	virtual void render() override;

	GfxMeshPtr getGfxMesh() { return _gfxMesh; }
	GfxShaderVertexPtr getVertexShader() { return _vertexShader; }
protected:
	template<class VertexType, class IndiceType>
	bool init(const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices, const std::string& vsFilename);
	template<class VertexType>
	bool init(const std::vector<VertexType>& vertexs, const std::string& vsFilename);
	MeshComponent();
	GfxMeshPtr _gfxMesh;
	GfxShaderVertexPtr _vertexShader;
};

template<class VertexType, class IndiceType>
MeshComponentPtr MeshComponent::create(const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices, const std::string& vsFilename)
{
	MeshComponent* ret = new MeshComponent();
	if (!ret || !ret->init(vertexs, indices,vsFilename))
		TINY_SAFE_DELETE(ret);
	return MeshComponentPtr(ret);
}

template<class VertexType>
MeshComponentPtr MeshComponent::create(const std::vector<VertexType>& vertexs, const std::string & vsFilename)
{
	MeshComponent* ret = new MeshComponent();
	if (!ret || !ret->init(vertexs, vsFilename))
		TINY_SAFE_DELETE(ret);
	return MeshComponentPtr(ret);
}

template<class VertexType, class IndiceType>
MeshComponentPtr MeshComponent::create(const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices)
{
	MeshComponent* ret = new MeshComponent();
	if (!ret || !ret->init(vertexs, indices,""))
		TINY_SAFE_DELETE(ret);
	return MeshComponentPtr(ret);
}

template<class VertexType>
MeshComponentPtr MeshComponent::create(const std::vector<VertexType>& vertexs)
{
	MeshComponent* ret = new MeshComponent();
	if (!ret || !ret->init(vertexs, ""))
		TINY_SAFE_DELETE(ret);
	return MeshComponentPtr(ret);
}

template<class VertexType, class IndiceType>
bool MeshComponent::init(const std::vector<VertexType>& vertexs, const std::vector<IndiceType>& indices, const std::string& vsFilename)
{
	do
	{
		static_assert(sizeof(IndiceType) == 2 || sizeof(IndiceType) == 4, "You can only use 16 bit or 32 bit type to save index buffer");
		const VertexInputlayoutDescription& vertexDescription = VertexType::getInputLayoutDescription();
		
		_gfxMesh = GfxMesh::create(
			GraphicMgr::instance()->getInputLayout<VertexType>(),
			vertexs.data(),
			(int)vertexs.size() * sizeof(VertexType),
			sizeof(VertexType),
			indices.data(),
			(int)indices.size() * sizeof(IndiceType),
			(sizeof(IndiceType) == 2 ? IndexBufferDataFormat::BIT_16 : IndexBufferDataFormat::BIT_32)
			);
		TINY_BREAK_IF(!_gfxMesh.isValid());

		if (vsFilename.empty())
			_vertexShader = nullptr;
		else
		{
			_vertexShader = ShaderMgr::instance()->getVSShader(vsFilename);
			TINY_BREAK_IF(!_vertexShader.isValid());
		}

		return true;
	} while (0);
	return false;
}

template<class VertexType>
bool MeshComponent::init(const std::vector<VertexType>& vertexs, const std::string& vsFilename)
{
	if (vertexs.size() > (size_t)(uint16_t)-1)
	{
		std::vector<uint32_t> indices(vertexs.size());
		for (int i = 0; i < (int)vertexs.size(); ++i)
			indices[i] = i;
		return init(vertexs, indices, vsFilename);
	}
	else
	{
		std::vector<uint16_t> indices(vertexs.size());
		for (int i = 0; i < (int)vertexs.size(); ++i)
			indices[i] = i;
		return init(vertexs, indices, vsFilename);
	}
}

