#include "precomp.h"
#include "CommandLineCfg.h"
#include <regex>
#include "TinyEngine/TinyEngine.h"

std::map<std::string, CommandLineHelper > CommandLineHelper::s_commandLines;
CommandLineCfg* CommandLineCfg::s_instance = nullptr;

bool CommandLineCfg::createInstance(const char* commandLine)
{
	CommandLineCfg* instance = new CommandLineCfg();
	if(!instance || !instance->init(instance, commandLine))
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

void CommandLineCfg::destroy()
{
	TINY_SAFE_DELETE(s_instance);
}

bool CommandLineCfg::init(CommandLineCfg* instance, const char* commandLine)
{
	std::vector<std::string> tokens = spliteCommandLine(commandLine);
	for (auto tokenIt = tokens.begin(); tokenIt != tokens.end();)
	{
		std::string& symbol = *tokenIt;
		std::transform(symbol.begin(), symbol.end(), symbol.begin(), tolower);
		if (symbol[0] == '/')
		{
			symbol = symbol.substr(1);
		}
		auto it = CommandLineHelper::s_commandLines.find(symbol);
		++tokenIt;
		if (it != CommandLineHelper::s_commandLines.end() && it->second._settingFun)
		{
			std::string value = (tokenIt != tokens.end() ? *tokenIt : "");
			if (value[0] == '/')
			{
				value = "";
			}
			else if(value[0] == '\"')
			{
				value = value.substr(1);
			}
			if(value.length() && value[value.length() - 1] == '\"')
			{
				value = value.substr(0, value.length() - 1);
			}
			bool settingResult = it->second._settingFun(instance, value);
			if (!settingResult)
			{
				TinyAssert(false, "Commandline error: [%s]", symbol.c_str());
				return false;
			}
		}
	}
	return true;
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
