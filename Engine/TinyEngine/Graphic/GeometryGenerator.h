#pragma once
#include "TinyEngine\Components\MeshComponent.h"
#include "Math\vector\Vector.h"

class GeometryGenerator
{
public:
	static GeometryGenerator* instance();
public:
	MeshComponentPtr createCubeMesh(InputLayoutType layoutType, float width, float height, float lenth);
	MeshComponentPtr createSphereMeshData(InputLayoutType layoutType, float radius, int numSubdivisions);
protected:
	void subdivideTriangle(std::vector<Vector3>& pos, std::vector<int>& indices);
};