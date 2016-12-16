#pragma once
#include "Math\vector\Vector.h"
#include "Graphic\Vertex\CommonVertex.h"

TINY_DEFINE_PTR(Object);
class File;

class WavefrontObjReader
{
public:
	WavefrontObjReader(const std::string& fileName);
public:
	bool readObjFile(std::vector<ObjectPtr>& outObj);
protected:
	bool readToken();
	bool parseToken();
	bool isBlankChar(char chara);
	
	void handleO();
	void handleV();
	void handleVN();
	void handleVT();
	void handleF();
	void handleUSEMTL();
	void handleS();
	void handleMTLLIB();
	void finishedObj();


	std::string _fileName;
	std::vector<char> _fileData;
	int _currentPos;

	std::string _currentToken;
	std::vector<ObjectPtr> _pawnObj;
	ObjectPtr _currentObj;

	std::vector<Vector3> _tempVecArr;
	std::vector<Vector3> _tempNormArr;
	std::vector<Vector2> _tempTexArr;

	std::vector<CommonVertex> _meshVertex;
	std::vector<GfxMaterialPtr> _materials;
};