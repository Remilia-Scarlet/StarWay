#define _CRT_SECURE_NO_WARNINGS
#include "ShaderCompiler.h"
#include <fstream>
#include "Engine/TinyEngine/ThirdParty/rapidjson/document.h"
#include "Engine/TinyEngine/ThirdParty/rapidjson/writer.h"
#include "Engine/TinyEngine/ThirdParty/rapidjson/fwd.h"
#include "Engine/TinyEngine/ThirdParty/rapidjson/stringbuffer.h"
#include "Engine/TinyEngine/ThirdParty/rapidjson/prettywriter.h"
#include "Engine/Ash/FileSystem/File_Win.h"
#include "Engine/TinyEngine/Engine/EngineDefs.h"

#include "Engine/Ash/CommonFunc.h"
#include <list>
#include <stack>
#include <cstdlib>
#include <regex>
#include "../../../Engine/TinyEngine/ThirdParty/rapidjson/error/en.h"
#include <iostream>

using namespace rapidjson;


ShaderCompiler::ShaderCompiler()
{
}


ShaderCompiler::~ShaderCompiler()
{
}

int ShaderCompiler::run(const std::string& cmdLine)
{
	if (!parseArg(cmdLine))
		return -1;

	Path sourcePath(_cmdLine.getSource());
	std::string fileName = sourcePath.getFileName();
	std::string shaderType;
	if (fileName.find("_PS") != -1)
		shaderType = "ps";
	else if (fileName.find("_VS") != -1)
		shaderType = "vs";
	
	Path fxcPath("game:fxc.exe");
	std::string cmd = fxcPath.getAbsolutePath();

	std::string param = std::string("\"") + cmd + std::string("\" /T ") + shaderType + "_5_0 /E main /O3 /Zi /Fo \"" + _cmdLine.getOutput() + "\" /nologo \"" + _cmdLine.getSource() + "\" ";


	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;
	BOOL result = CreateProcess(
		cmd.data(),
		(char*)param.c_str(),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	);
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitCode = 1;
	if(!GetExitCodeProcess(pi.hProcess, &exitCode))
	{
		exitCode = 1;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return exitCode;
}

bool ShaderCompiler::parseArg(const std::string& cmdLine)
{
	return _cmdLine.init(cmdLine.c_str());
}
