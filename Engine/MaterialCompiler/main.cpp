#include "MaterialCompiler/precomp.h"
#include <Windows.h>
#include "Ash/CommonFunc.h"
#include "TinyEngine/Graphic/Material.h"
#include "Math/vector/Vector.h"
#include <memory>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <sstream>

// Temp solution to compile material
// Later this part will be moved to tool mode.

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	std::vector<std::unique_ptr<GraphicCommand>> commands;
	commands.push_back(std::make_unique<SetPsLocalConstantCmd>("default_material", "ambient", Vector4(0, 0, 0, 0)));
	commands.push_back(std::make_unique<SetPsLocalConstantCmd>("default_material", "diffuse", Vector4(0.64f, 0.000654f, 0.f, 0.f)));
	commands.push_back(std::make_unique<SetPsLocalConstantCmd>("default_material", "specular", Vector4(0.5f, 0.5f, 0.5f, 0.f)));
	commands.push_back(std::make_unique<SetPsLocalConstantCmd>("default_material", "emit", Vector4(0, 0, 0, 0)));
	MaterialPtr mat1 = Material::create("default_material", commands);

	mat1->saveOrigin("game:123.matori");

	MaterialPtr mat2 = Material::create("game:123.matori");



	return 0;
}
