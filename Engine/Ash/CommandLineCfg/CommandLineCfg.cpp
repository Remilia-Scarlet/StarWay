#include "precomp.h"
#include "CommandLineCfg.h"
#include <regex>
#include "CommonFunc.h"
#include "TinyAssert.h"
#include <ppltasks.h>

bool CommandLineCfg::init(const char* commandLine)
{
	spliteCommandLine(commandLine);
	return _initImp();
}

bool CommandLineCfg::init(int argc, char * argv[])
{
	for (int i = 0; i < argc; ++i)
	{
		_splitedCommandLine.push_back(argv[i]);
	}
	return _initImp();
}

bool CommandLineCfg::_initImp()
{
	for (auto tokenIt = _splitedCommandLine.begin(); tokenIt != _splitedCommandLine.end();)
	{
		std::string& symbol = *tokenIt;
		std::transform(symbol.begin(), symbol.end(), symbol.begin(), tolower);
		if (symbol[0] == '/')
		{
			symbol = symbol.substr(1);
		}
		auto it = _registedCommandLines.find(symbol);
		++tokenIt;
		if (it != _registedCommandLines.end() && it->second._settingFun)
		{
			std::string value = (tokenIt != _splitedCommandLine.end() ? *tokenIt : "");
			if (value[0] == '/')
			{
				value = "";
			}
			else if (value[0] == '\"')
			{
				value = value.substr(1);
			}
			if (value.length() && value[value.length() - 1] == '\"')
			{
				value = value.substr(0, value.length() - 1);
			}
			bool settingResult = it->second._settingFun(value);
			if (!settingResult)
			{
				TinyAssert(false, "Commandline error: [%s]", symbol.c_str());
				return false;
			}
		}
	}
	return true;
}

void CommandLineCfg::spliteCommandLine(const char* commandLine)
{
	_splitedCommandLine.clear();
	std::regex nameRegex("([-/a-zA-Z0-9_]+)|(\"[^\"]+\")");
	std::smatch matchResult;
	std::string cmdLine(commandLine);
	//find first symbol in define.
	while (std::regex_search(cmdLine, matchResult, nameRegex))
	{
		std::string str = matchResult.str();
		_splitedCommandLine.push_back(str);
		cmdLine = matchResult.suffix();
	}
}
