#include "precomp.h"
#include "TinyEngine/Graphic/GeometryGenerator.h"
#include "Graphic/Vertex/InputLayoutDefine.h"
#include "Graphic/Vertex/DefaultVertex.h"

GeometryGenerator* GeometryGenerator::instance()
{
	static GeometryGenerator s_instance;
	return &s_instance;
}

MeshComponentPtr GeometryGenerator::createCubeMesh(float width, float height, float lenth)
{
	float w2 = width / 2.f;
	float h2 = height / 2.f;
	float d2 = lenth / 2.f;

	std::vector<short> indices = {3,  1,  0, 2,  1,  3, //top
								  6,  4,  5, 7,  4,  6, //bottom
								  11, 9,  8,  10, 9,  11,
								  14, 12, 13, 15, 12, 14,
								  19, 17, 16, 18, 17, 19,
								  22, 20, 21, 23, 20, 22};

	std::vector<CommonVertex> vertexBuffer = {
		CommonVertex({-w2, h2, -d2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}),   CommonVertex({w2, h2, -d2}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}),
		CommonVertex({w2, h2, d2}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}),	 CommonVertex({-w2, h2, d2}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}),
		CommonVertex({-w2, -h2, -d2}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}), CommonVertex({w2, -h2, -d2}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}),
		CommonVertex({w2, -h2, d2}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}),   CommonVertex({-w2, -h2, d2}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}),
		CommonVertex({-w2, -h2, d2}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),  CommonVertex({-w2, -h2, -d2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
		CommonVertex({-w2, h2, -d2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),  CommonVertex({-w2, h2, d2}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
		CommonVertex({w2, -h2, d2}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),	CommonVertex({w2, -h2, -d2}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
		CommonVertex({w2, h2, -d2}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),	CommonVertex({w2, h2, d2}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
		CommonVertex({-w2, -h2, -d2}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}), CommonVertex({w2, -h2, -d2}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}),
		CommonVertex({w2, h2, -d2}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}),   CommonVertex({-w2, h2, -d2}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}),
		CommonVertex({-w2, -h2, d2}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}),   CommonVertex({w2, -h2, d2}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}),
		CommonVertex({w2, h2, d2}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}),	 CommonVertex({-w2, h2, d2}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}),
	};
	return MeshComponent::create(vertexBuffer, indices);

	TinyAssert(false, "unreachable code");
	return MeshComponentPtr();
}

MeshComponentPtr GeometryGenerator::createSphereMeshData(float radius, int numSubdivisions)
{
	// Put a cap on the number of subdivisions.
	numSubdivisions = min(numSubdivisions, 5);

	// Approximate a sphere by tessellating an icosahedron.

	const float X = 0.525731f;
	const float Z = 0.850651f;

	std::vector<Vector3> pos =
	{
		{-X, 0.0f, Z},	{X, 0.0f, Z},
		{-X, 0.0f, -Z},	{X, 0.0f, -Z},
		{0.0f, Z, X},	{0.0f, Z, -X},
		{0.0f, -Z, X},	{0.0f, -Z, -X},
		{Z, X, 0.0f},	{-Z, X, 0.0f},
		{Z, -X, 0.0f},	{-Z, -X, 0.0f}
	};

	std::vector<int> indices =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	for (int i = 0; i < numSubdivisions; ++i)
		subdivideTriangle(pos,indices);

	std::vector<CommonVertex> vertexBuffer;
	vertexBuffer.reserve(pos.size());
	// Project vertices onto sphere and scale.
	for (int i = 0; i < int(pos.size()); ++i)
	{
		// Project onto unit sphere.
		Vector4 normal(pos[i].X(), pos[i].Y(), pos[i].Z(), 0.f);
		normal.normalizeInPlace();

		// Project onto sphere.
		Vector4 p = radius * normal;

		float theta = atan2(p.X(), p.Z());
		float phi = acosf(p.Y() / radius);

		vertexBuffer.push_back(CommonVertex(p.XYZ(), normal.XYZ(), Vector2(theta / float(2 * M_PI), phi / float(M_PI))));
	}
	return MeshComponent::create(vertexBuffer, indices);
}

void GeometryGenerator::subdivideTriangle(std::vector<Vector3>& pos, std::vector<int>& indices)
{
	// Save a copy of the input geometry.
	std::vector<Vector3> posCopy = pos;
	std::vector<int> indicesCopy = indices;

	pos.clear();
	indices.clear();

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	int numTris = int(indicesCopy.size()) / 3;
	for (int i = 0; i < numTris; ++i)
	{
		Vector3 v0 = posCopy[indicesCopy[i * 3 + 0]];
		Vector3 v1 = posCopy[indicesCopy[i * 3 + 1]];
		Vector3 v2 = posCopy[indicesCopy[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vector3 m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0 = Vector3(
			0.5f*(v0.X() + v1.X()),
			0.5f*(v0.Y() + v1.Y()),
			0.5f*(v0.Z() + v1.Z()));

		m1 = Vector3(
			0.5f*(v1.X() + v2.X()),
			0.5f*(v1.Y() + v2.Y()),
			0.5f*(v1.Z() + v2.Z()));

		m2 = Vector3(
			0.5f*(v0.X() + v2.X()),
			0.5f*(v0.Y() + v2.Y()),
			0.5f*(v0.Z() + v2.Z()));

		//
		// Add new geometry.
		//

		pos.push_back(v0); // 0
		pos.push_back(v1); // 1
		pos.push_back(v2); // 2
		pos.push_back(m0); // 3
		pos.push_back(m1); // 4
		pos.push_back(m2); // 5

		indices.push_back(i * 6 + 0);
		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 5);

		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 5);

		indices.push_back(i * 6 + 5);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 2);

		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 1);
		indices.push_back(i * 6 + 4);
	}
}
