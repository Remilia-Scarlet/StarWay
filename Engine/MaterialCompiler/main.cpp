#include "MaterialCompiler/precomp.h"
#include <Windows.h>
#include "Ash/CommonFunc.h"
#include "TinyEngine/Graphic/Material.h"

// Temp solution to compile material
// Later this part will be moved to tool mode.

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MaterialSaver mat1;
	mat1._shader = "default_material";
	GraphicCommandSaver graphicCmd;
	graphicCmd._commandType = GraphicCommand::CommandType::SET_LOCAL_PS_CONSTANT;

	mat1._command.push_back();



	return 0;
}
