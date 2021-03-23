#pragma once
#include "DangoEngine/Components/MeshComponent.h"
#include "DangoMath/vector/Vector.h"

class GeometryGenerator
{
public:
	static GeometryGenerator* instance();
public:
	MeshComponentPtr createCubeMesh(float width, float height, float lenth);
	MeshComponentPtr createSphereMeshData(float radius, int numSubdivisions);
protected:
	void subdivideTriangle(std::vector<Vector3>& pos, std::vector<int>& indices);
};