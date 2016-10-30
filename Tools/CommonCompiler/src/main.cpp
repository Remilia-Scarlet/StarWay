#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>

#include "CommonCompiler.h"
#include "Engine\Ash\CommonFunc.h"
#include <time.h>

void showHelp()
{
	DebugString("Useage:");
	DebugString("CommonCompiler [/? | /config configFilePath | /force]");
	DebugString("\tWhere");
	DebugString("\t\t/?\tShow this menu");
	DebugString("\t\t/config\tGive the configFilePath. Config file is a json file.");
	DebugString("\t\t/force\tForce recompile all files\n");
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
	new int(0x74736574);

	const time_t t = time(NULL);
	struct tm* current_time = localtime(&t);
	DebugString("====================Common compiler start. %s", asctime(current_time));

	CommonCompiler compiler;
	int result = compiler.run(argc, argv);
	switch (result)
	{
	case 0:
		DebugString("^^^^^^^^^^^^^^^^^^^^Finished succeed!");
		break;
	case 1:
		showHelp();
		DebugString("^^^^^^^^^^^^^^^^^^^^Finished");
		system("pause");
		break;
	default:
		DebugString("^^^^^^^^^^^^^^^^^^^^Finished failed! Check \"log.txt\" for more infomation");
		system("pause");
		break;
	}
	return result;
}