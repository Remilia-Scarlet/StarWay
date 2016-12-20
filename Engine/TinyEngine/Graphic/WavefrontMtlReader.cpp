#include "TinyEngine/precomp.h"
#include "TinyEngine/Graphic/WavefrontMtlReader.h"

WavefrontMtlReader::WavefrontMtlReader(const std::string& fileName)
	:_parser(fileName)
{
	registerCommand();
}

bool WavefrontMtlReader::readMtlFile(std::map<std::string, TextureComponentPtr>& outTexture)
{
	outTexture.clear();

	bool result = _parser.startParse();
	outTexture = std::move(_textures);
	return true;
}

void WavefrontMtlReader::registerCommand()
{
	_parser.registerCommand("newmtl", std::bind(&WavefrontMtlReader::handleNEWMTL, this));
	_parser.registerCommand("Ns", std::bind(&WavefrontMtlReader::handleNS, this));
	_parser.registerCommand("Ka", std::bind(&WavefrontMtlReader::handleKA, this));
	_parser.registerCommand("Kd", std::bind(&WavefrontMtlReader::handleKD, this));
	_parser.registerCommand("Ks", std::bind(&WavefrontMtlReader::handleKS, this));
	_parser.registerCommand("illum", std::bind(&WavefrontMtlReader::handleILLUM, this));
	_parser.registerCommand("map_Kd", std::bind(&WavefrontMtlReader::handleMAP_KD, this));
}

void WavefrontMtlReader::handleNEWMTL()
{
	GfxMaterialPtr mat = GfxMaterial::create();
	_materials[_parser.nextParam()] = mat;
	_currentMat = mat;
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

void WavefrontMtlReader::handleILLUM()
{

}

void WavefrontMtlReader::handleMAP_KD()
{
	std::string name = _parser.nextParam();
	TextureComponentPtr tex = TextureComponent::create(name, "");
	_textures[name] = tex;
}
