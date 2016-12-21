#pragma once
#include "WavefrontParser.h"
#include <map>
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Graphic/gfx/GfxMaterial.h"
#include "TinyEngine/Components/TextureComponent.h"

class WavefrontMtlReader
{
public:
	WavefrontMtlReader(const std::string& fileName);
public:
	bool readMtlFile(std::map<std::string, TextureComponentPtr>& outTexture);
protected:
	void registerCommand();

	void handleNEWMTL();
	void handleNS();
	void handleKA();
	void handleKD();
	void handleKS();
	void handleILLUM();
	void handleMAP_KD();

	void finisheMtl();

	WavefrontParser _parser;
	std::map<std::string, TextureComponentPtr> _components;
	std::string _currentName;

	std::map<std::string, GfxTexturePtr> _textures;
	GfxTexturePtr _currentTex;
	GfxMaterialPtr _currentMat;
};