#include "TinyEngine/precomp.h"
#include "TinyEngine/Graphic/WavefrontObjReader.h"
#include "Ash/FileSystem/fs_include.h"
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
	if (!result)
		return false;

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
}

void WavefrontObjReader::handleV()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	float z = std::stof(_parser.nextParam());
	_tempVecArr.push_back({ x, y, -z });
}

void WavefrontObjReader::handleVN()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	float z = std::stof(_parser.nextParam());
	_tempNormArr.push_back({ x, y, -z });
}

void WavefrontObjReader::handleVT()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	_tempTexArr.push_back({x, 1 - y });
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
	_mtlName = _parser.nextParam();
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
		MeshComponentPtr mesh = MeshComponent::create(_meshVertex);
		mesh->setPrimitiveTopology(PrimitiveTopology::TRIANGLE_LIST);
		_currentObj->addComponent(mesh);
		_meshVertex.clear();

		auto it = _materials.find(_mtlName);
		MaterialComponentPtr texture = nullptr;
		if (it == _materials.end())
			texture = MaterialComponent::create();
		else
			texture = it->second;
		_currentObj->addComponent(texture);
		_mtlName.clear();

		_pawnObj.push_back(_currentObj);
	}
}

void WavefrontObjReader::readMaterialFile(const std::string& filename)
{
	//WavefrontMtlReader matReader(std::string("game:") + filename);
	//std::map<std::string,MaterialComponentPtr> outMtl;
	//matReader.readMtlFile(outMtl);
	//_materials = std::move(outMtl);
}
