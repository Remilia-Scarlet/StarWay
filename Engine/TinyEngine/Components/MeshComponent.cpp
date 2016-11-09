#include "TinyEngine\precomp.h"
#include "MeshComponent.h"
MeshComponent::MeshComponent()
	:BaseComponent(TO_STRING(MeshComponent))
{
}


bool MeshComponent::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_END(MeshComponent);
	return true;
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::setMaterial(const Material& material)
{
	_gfxMesh->setMaterial(material);
}

const Material& MeshComponent::getMaterial()
{
	return _gfxMesh->getMaterial();
}

void MeshComponent::setPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
	_gfxMesh->setPrimitiveTopology(primitiveTopology);
}

PrimitiveTopology MeshComponent::getPrimitiveTopology() const
{
	return _gfxMesh->getPrimitiveTopology();
}

void MeshComponent::render()
{
	_gfxMesh->render(_vertexShader);
	_vertexShader->render();
}
