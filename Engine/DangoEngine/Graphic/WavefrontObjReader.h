#pragma once
#include "Math\vector\Vector.h"
#include "Graphic\Vertex\DefaultVertex.h"
#include "WavefrontParser.h"

ASH_DEFINE_PTR(Object);
ASH_DEFINE_PTR(MaterialComponent);

class WavefrontObjReader
{
public:
	WavefrontObjReader(const std::string& fileName);
public:
	bool readObjFile(std::vector<ObjectPtr>& outObj);
protected:
	void registerCommand();
	void handleO();
	void handleV();
	void handleVN();
	void handleVT();
	void handleF();
	void handleUSEMTL();
	void handleMTLLIB();

	void finishedObj();
	void readMaterialFile(const std::string& filename);

	WavefrontParser _parser;

	std::vector<ObjectPtr> _pawnObj;
	ObjectPtr _currentObj;

	std::vector<Vector3> _tempVecArr;
	std::vector<Vector3> _tempNormArr;
	std::vector<Vector2> _tempTexArr;

	std::vector<CommonVertex> _meshVertex;
	std::map<std::string, MaterialComponentPtr> _materials;
	std::string _mtlName;
};