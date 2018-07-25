#include "precomp.h"
#define _CRT_SECURE_NO_WARNINGS
#include "ShaderCompiler.h"
#include "Engine/Ash/CommonFunc.h"

void showHelp()
{
	DebugString("Useage:");
	DebugString(MyCommandLineCfg().generateHelpString().c_str());
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
	new int(0x74736574);

	std::string cmdLine = GetCommandLineA();// wcharToChar(lpCmdLine);

	//printf(cmdLine.c_str());
	//printf("\n");

	ShaderCompiler compiler;
	int result = compiler.run(cmdLine);
	switch (result)
	{
	case 0: // succceed
		printf("Compiling shader succeed\n");
		break;
	case -1://arg error
		showHelp();
		break;
	case 1:// failed due to error duing compilling
	default: // failed due to other issues
		//printf("Compiling shader failed\n");
		break;
	}
	
	return result;
}