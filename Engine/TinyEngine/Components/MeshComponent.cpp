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

void MeshComponent::render()
{
	_gfxMesh->render(_vertexShader);
	_vertexShader->render();
}
