#include "precomp.h"
#include "MeshComponent.h"
#include "Graphic\Vertex\DefaultVertex.h"
#include "Graphic\Vertex\InputLayoutDefine.h"
#include "Graphic\Manager\DefaultMgr.h"
MeshComponent::MeshComponent()
	:BaseComponent(TO_STRING(MeshComponent))
{
}


MeshComponent::~MeshComponent()
{
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
	if (_vertexShader.isValid())
		_vertexShader->render();
	else
		DefaultMgr::instance()->getDefaultVS()->render();
}
