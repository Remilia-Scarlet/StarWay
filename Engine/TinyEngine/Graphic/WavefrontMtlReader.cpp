#include "precomp.h"
#include "TinyEngine/Graphic/WavefrontMtlReader.h"
#include "Ash/FileSystem/File_Win.h"

WavefrontMtlReader::WavefrontMtlReader(const std::string& fileName)
	:_parser(fileName)
{
	registerCommand();
}

bool WavefrontMtlReader::readMtlFile(std::map<std::string, TextureComponentPtr>& outTexture)
{
	outTexture.clear();

	bool result = _parser.startParse();
	if (!result)
		return false;
	finisheMtl();
	outTexture = std::move(_components);
	return true;
}

void WavefrontMtlReader::registerCommand()
{
	_parser.registerCommand("newmtl", std::bind(&WavefrontMtlReader::handleNEWMTL, this));
	_parser.registerCommand("Ns", std::bind(&WavefrontMtlReader::handleNS, this));
	_parser.registerCommand("Ka", std::bind(&WavefrontMtlReader::handleKA, this));
	_parser.registerCommand("Kd", std::bind(&WavefrontMtlReader::handleKD, this));
	_parser.registerCommand("Ks", std::bind(&WavefrontMtlReader::handleKS, this));
	_parser.registerCommand("Ke", std::bind(&WavefrontMtlReader::handleKE, this));
	_parser.registerCommand("illum", std::bind(&WavefrontMtlReader::handleILLUM, this));
	_parser.registerCommand("map_Kd", std::bind(&WavefrontMtlReader::handleMAP_KD, this));
}

void WavefrontMtlReader::handleNEWMTL()
{
	finisheMtl();
	_currentMat = GfxMaterial::create();
	_currentName = _parser.nextParam();
}

void WavefrontMtlReader::handleNS()
{
	Vector4 specular = _currentMat->getSpecular();
	specular.W() = std::stof(_parser.nextParam());
	_currentMat->setSpecular(specular);
}

void WavefrontMtlReader::handleKA()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	float z = std::stof(_parser.nextParam());
	_currentMat->setAmbient({ x,y,z,1 });
}

void WavefrontMtlReader::handleKD()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	float z = std::stof(_parser.nextParam());
	_currentMat->setDiffuse({ x,y,z,1 });
}

void WavefrontMtlReader::handleKS()
{
	Vector4 specular = _currentMat->getSpecular();
	specular.X() = std::stof(_parser.nextParam());
	specular.Y() = std::stof(_parser.nextParam());
	specular.Z() = std::stof(_parser.nextParam());
	_currentMat->setSpecular(specular);
}

void WavefrontMtlReader::handleKE()
{
	float x = std::stof(_parser.nextParam());
	float y = std::stof(_parser.nextParam());
	float z = std::stof(_parser.nextParam());
	_currentMat->setEmit({ x,y,z,1 });
}

void WavefrontMtlReader::handleILLUM()
{

}

void WavefrontMtlReader::handleMAP_KD()
{
	std::string name = _parser.nextParam();
	auto it = _textures.find(name);
	if (it == _textures.end())
	{
		File file;
		bool result = file.open("game:" + name, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
		if (!result)
			return;
		std::vector<char> data = file.readAll();
		file.close();
		GfxTexturePtr tex = GfxTexture::create((uint8_t*)data.data(), (int)data.size(), name.c_str());
		_textures[name] = tex;
		_currentTex = tex;
	}
	else
	{
		_currentTex = it->second;
	}
}

void WavefrontMtlReader::finisheMtl()
{
	if (_currentName.size() > 0)
	{
		TextureComponentPtr texCom = TextureComponent::create();
		texCom->setMaterial(_currentMat);
		texCom->setTexture(_currentTex);
		_components[_currentName] = texCom;
	}
	_currentName.clear();
	_currentTex = nullptr;
	_currentMat = nullptr;
}
