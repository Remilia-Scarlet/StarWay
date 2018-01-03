#include "precomp.h"
#include "CommandLineCfg.h"
#include <regex>

std::map<std::string, CommandLineHelper > CommandLineHelper::s_commandLines;
CommandLineCfg* CommandLineCfg::s_instance = nullptr;

bool CommandLineCfg::createInstance(const char* commandLine)
{
	CommandLineCfg* instance = new CommandLineCfg();
	if(!instance || !instance->init(commandLine))
	{
		TINY_SAFE_DELETE(instance);
		TinyAssert(false, "CommandLineCfg init failed. Please check commandline, use /? to get all commanline");
		return false;
	}
	s_instance = instance;
	return true;
}

CommandLineCfg* CommandLineCfg::instance()
{
	TinyAssert(s_instance,"CommandLineCfg haven't been initialized");
	return s_instance;
}

bool CommandLineCfg::init(const char* commandLine)
{
	spliteCommandLine(commandLine);
	std::string errorCommand;
	do
	{
		std::string strCmd(commandLine);
		strCmd += " ";
		std::regex nameRegex("/([\\w]+) +");
		//std::regex valueRegex(R"([(\\w+ +)|(".*" +)])");
		std::regex valueRegex(R"((\\w+ +)|(".*" +))");

		std::smatch matchResult;

		//find first symbol in define.
		while (std::regex_search(strCmd, matchResult, nameRegex))
		{
			std::string symbol = matchResult[1].str();
			std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);
			strCmd = matchResult.suffix();
			bool haveValue = std::regex_search(strCmd, matchResult, valueRegex);

			auto it = CommandLineHelper::s_commandLines.find(symbol);
			if(it != CommandLineHelper::s_commandLines.end() && it->second._settingFun)
			{
				bool settingResult = CommandLineHelper::s_commandLines[symbol]._settingFun(haveValue ? matchResult.str() : "");
				if(!settingResult)
				{
					errorCommand = symbol;
					break;
				}
			}
		}
		if(errorCommand.length() == 0)
			return true;
	} while (false);
	TinyAssert(false, "Commanline failed when parsing %s", errorCommand.c_str());
	return false;
}

std::vector<std::string> CommandLineCfg::spliteCommandLine(const char* commandLine)
{
	std::vector<std::string> ret;
	std::regex nameRegex("([-/a-zA-Z0-9_]+)|(\"[^\"]+\")");
	std::smatch matchResult;
	std::string cmdLine(commandLine);
	//find first symbol in define.
	while (std::regex_search(cmdLine, matchResult, nameRegex))
	{
		std::string str = matchResult.str();
		ret.push_back(str);
		cmdLine = matchResult.suffix();
	}
	return ret;
}
