#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

#include "ShaderCompiler.h"
#include "Engine/Ash/CommonFunc.h"
#include <ctime>

void showHelp()
{
	DebugString("Useage:");
	DebugString(R"(ShaderCompiler [/? | /Source:"Hlsl path" | /Output:"Output file"])");
	DebugString("\tWhere");
	DebugString("\t\t/?\tShow this menu");
	DebugString("\t\t/Source\tGive the source file path.\n");
	DebugString("\t\t/Output\tGive the output file\n");
}

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
	new int(0x74736574);

	//AllocConsole();
	//freopen("CONIN$", "r", stdin);
	//freopen("CONOUT$", "w", stdout);
	//freopen("CONOUT$", "w", stderr);
	//printf("Console Started\n");
	//wchar_t* lpCmdLine = L"/Source:\"E:\\dango\\document\\Visual Studio Projects\\StarWay\\Engine\\CompileShader\\Shader\\DefaultShader_NoTex_PS.hlsl\" /Output:\"123.ooo\"";
	std::string cmdLine = GetCommandLineA();// wcharToChar(lpCmdLine);

	ShaderCompiler compiler;
	int result = compiler.run(cmdLine);
	switch (result)
	{
	case 0: // succceed
		break;
	case 1:// failed due to fxc returns 1
		break;
	default: // failed due to other issues
		showHelp();
		break;
	}
	return result;
}