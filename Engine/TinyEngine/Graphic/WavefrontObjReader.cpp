#include "TinyEngine/precomp.h"
#include "TinyEngine/Graphic/WavefrontObjReader.h"
#include "Ash/FileSystem/File_Win.h"
#include "TinyEngine/Objects/Object.h"
#include <string>
#include "TinyEngine/Components/MeshComponent.h"
#include "Graphic/Vertex/InputLayoutDefine.h"

WavefrontObjReader::WavefrontObjReader(const std::string& fileName)
	: _fileName(fileName)
	, _currentPos(0)
{

}

bool WavefrontObjReader::readObjFile(std::vector<ObjectPtr>& outObj)
{
	outObj.clear();
	File file;
	bool result = file.open(_fileName, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	if (!result)
	{
		DebugString("Can't open %s for model loading", _fileName.c_str());
		return false;
	}
	_fileData = file.readAll();
	file.close();

	result = true;
	while(true)
	{
		result = readToken();
		TINY_BREAK_IF(!result);
		result = parseToken();
		TINY_BREAK_IF(!result);
	};
	finishedObj();
	outObj = std::move(_pawnObj);
	return true;
}

bool WavefrontObjReader::readToken()
{
	_currentToken.clear();
	int size = (int) _fileData.size();
	while (true)
	{
		if (_currentPos == _fileData.size())
			return _currentToken.size() > 0;
		char currentChar = _fileData[_currentPos];
		if (currentChar == '#')
		{
			if (_currentToken.size() > 0)
				return true;
			do
			{
				++_currentPos;
			} while (_currentPos < size && _fileData[_currentPos] != '\n');
			if (_currentPos < size)
				++_currentPos;
		}
		else if (isBlankChar(currentChar))
		{
			if (_currentToken.size() > 0)
				return true;
			++_currentPos;
			while (_currentPos <  size && isBlankChar(_fileData[_currentPos]))
			{
				++_currentPos;
			}
		}
		else
		{
			_currentToken.push_back(currentChar);
			++_currentPos;
		}
	}
}

bool WavefrontObjReader::parseToken()
{
	const static std::string o = "o";
	const static std::string v = "v";
	const static std::string vn = "vn";
	const static std::string vt = "vt";
	const static std::string f = "f";
	const static std::string usemtl = "usemtl";
	const static std::string s = "s";
	const static std::string mtllib = "mtllib";
	if (_currentToken == o)
	{
		handleO();
	}
	else if (_currentToken == v)
	{
		handleV();
	}
	else if (_currentToken == vn)
	{
		handleVN();
	}
	else if (_currentToken == vt)
	{
		handleVT();
	}
	else if (_currentToken == f)
	{
		handleF();
	}
	else if (_currentToken == usemtl)
	{
		handleUSEMTL();
	}
	else if (_currentToken == s)
	{
		handleS();
	}
	else if (_currentToken == mtllib)
	{
		handleMTLLIB();
	}
	else
	{
		TinyAssert(false, "unknown command");
		return false;
	}
	return true;
}

bool WavefrontObjReader::isBlankChar(char chara)
{
	return chara == '\n' || chara == ' ' || chara == '\t';
}

void WavefrontObjReader::handleO()
{
	finishedObj();
	readToken();
	_currentObj = Object::create(_currentToken);
	_meshVertex.clear();
}

void WavefrontObjReader::handleV()
{
	readToken();
	float x = std::stof(_currentToken);
	readToken();
	float y = std::stof(_currentToken);
	readToken();
	float z = std::stof(_currentToken);
	_tempVecArr.push_back({ x,y,z });
}

void WavefrontObjReader::handleVN()
{
	readToken();
	float x = std::stof(_currentToken);
	readToken();
	float y = std::stof(_currentToken);
	readToken();
	float z = std::stof(_currentToken);
	_tempNormArr.push_back({ x,y,z });
}

void WavefrontObjReader::handleVT()
{
	readToken();
	float x = std::stof(_currentToken);
	readToken();
	float y = std::stof(_currentToken);
	_tempTexArr.push_back({ x,y });
}

void WavefrontObjReader::handleF()
{
	for (int i = 0; i < 3; ++i)
	{
		readToken();
		int tempArr[3] = { 0 };
		int tempArrIndex = 0;
		for (size_t i = 0; i < _currentToken.length(); ++i)
		{
			char currentChar = _currentToken[i];
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
	readToken();
}

void WavefrontObjReader::handleS()
{
	readToken();
}

void WavefrontObjReader::handleMTLLIB()
{
	readToken();
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
