#include "TinyEngine\precomp.h"
#include "MeshComponent.h"
MeshComponent::MeshComponent()
	:BaseComponent()
{
}


MeshComponent::~MeshComponent()
{
}

void MeshComponent::render()
{
	_gfxMesh->render(_vertexShader);
	_vertexShader->render();
}
