#include "TinyEngine/precomp.h"
#include "TinyEngine/Graphic/WavefrontObjReader.h"
#include "Ash/FileSystem/File_Win.h"
#include "TinyEngine/Objects/Object.h"
#include <string>
#include "TinyEngine/Components/MeshComponent.h"
#include "Graphic/Vertex/InputLayoutDefine.h"

WavefrontObjReader::WavefrontObjReader(const std::string& fileName)
	: _parser(fileName)
{
	registerCommand();
}

bool WavefrontObjReader::readObjFile(std::vector<ObjectPtr>& outObj)
{
	outObj.clear();
	
	bool result = _parser.startParse();
	finishedObj();
	outObj = std::move(_pawnObj);
	return true;
}

void WavefrontObjReader::registerCommand()
{
	_parser.registerCommand("o", std::bind(&WavefrontObjReader::handleO, this));
	_parser.registerCommand("v", std::bind(&WavefrontObjReader::handleV, this));
	_parser.registerCommand("vt", std::bind(&WavefrontObjReader::handleVT, this));
	_parser.registerCommand("vn", std::bind(&WavefrontObjReader::handleVN, this));
	_parser.registerCommand("f", std::bind(&WavefrontObjReader::handleF, this));
	_parser.registerCommand("usemtl", std::bind(&WavefrontObjReader::handleUSEMTL, this));
	_parser.registerCommand("mtllib", std::bind(&WavefrontObjReader::handleMTLLIB, this));
}

void WavefrontObjReader::handleO()
{
	finishedObj();
	_currentObj = Object::create(_parser.nextParam());
	_meshVertex.clear();
}

void WavefrontObjReader::handleV()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	float z = std::stof(_parser.nextParam());
	_tempVecArr.push_back({ x,y,z });
}

void WavefrontObjReader::handleVN()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	float z = std::stof(_parser.nextParam());
	_tempNormArr.push_back({ x,y,z });
}

void WavefrontObjReader::handleVT()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	_tempTexArr.push_back({ x,y });
}

void WavefrontObjReader::handleF()
{
	// 3 points a face
	std::string token;
	for (int i = 0; i < 3; ++i)
	{
		token = _parser.nextParam();
		int tempArr[3] = { 0 };
		int tempArrIndex = 0;
		for (size_t i = 0; i < token.length(); ++i)
		{
			char currentChar = token[i];
			if (currentChar >= '0' && currentChar <= '9')
			{
				tempArr[tempArrIndex] *= 10;
				tempArr[tempArrIndex] += currentChar - '0';
			}
			else if (currentChar = '/')
			{
				++tempArrIndex;
			}
		}
		CommonVertex vec;
		if(tempArr[0] > 0)
			vec.pos = _tempVecArr[tempArr[0] - 1];
		if(tempArr[1] > 0)
			vec.uv = _tempTexArr[tempArr[1] - 1];
		if (tempArr[2] > 0)
			vec.normal = _tempNormArr[tempArr[2] - 1];
		_meshVertex.push_back(vec);
	}
}

void WavefrontObjReader::handleUSEMTL()
{
	std::string name = _parser.nextParam();
}

void WavefrontObjReader::handleMTLLIB()
{
	std::string name = _parser.nextParam();
	readMaterialFile(name);
}

void WavefrontObjReader::finishedObj()
{
	if (_currentObj.isValid())
	{
		MeshComponentPtr mesh = MeshComponent::create(InputLayoutType::COMMON, _meshVertex, "ShaderFirst_VS.cso");
		mesh->setPrimitiveTopology(PrimitiveTopology::TRIANGLE_LIST);
		_currentObj->addComponent(mesh);
		_pawnObj.push_back(_currentObj);
	}
}

void WavefrontObjReader::readMaterialFile(const std::string& filename)
{
	WavefrontObjReader matReader(filename);
	std::vector<ObjectPtr> outObj;
	matReader.readObjFile(outObj);
	_materials = std::move(matReader._materials);
}
