#include "precomp.h"
#include "CommandLineCfg.h"
#include "CommonFunc.h"
#include "TinyAssert.h"
#include <cctype>

bool CommandLineCfg::init(const char* commandLine)
{
	bool result = spliteCommandLine(commandLine);
	if (!result)
		return false;
	return _initImp();
}

bool CommandLineCfg::_initImp()
{
	for (auto tokenIt = _splitedCommandLine.begin(); tokenIt != _splitedCommandLine.end();++tokenIt)
	{
		const std::string& symbol = tokenIt->first;
		const std::string& value = tokenIt->second;
		
		auto it = _registedCommandLines.find(symbol);
		if (it != _registedCommandLines.end() && it->second._settingFun)
		{
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

bool CommandLineCfg::spliteCommandLine(const char* commandLine)
{
	_splitedCommandLine.clear();
	TinyAssert(commandLine);
	enum State
	{
		Start,
		CmdName,
		CmdValue
	};
	State stateMachine = Start;
	char curChr = 0;
	std::string curName;
	std::string curValue;
	bool isInQueto = false;
	do
	{
		curChr = *commandLine;
		switch (stateMachine)
		{
		case Start:
			if (curChr == '/')
			{
				stateMachine = CmdName;
				curName.clear();
			}
			break;
		case CmdName:
			if (isValidNameChar(curChr))
			{
				curName.push_back(std::tolower(curChr));
			}
			else if (curChr == ':')
			{
				stateMachine = CmdValue;
				curValue.clear();
				isInQueto = false;
			}
			else if ((curChr == 0 || curChr == ' ') && !curName.empty())
			{
				_splitedCommandLine[curName] = "";
			}
			else
			{
				TinyAssert(false, "Parsing command line failed");
				return false;
			}
			break;
		case CmdValue:
			if(curChr == '"')
			{
				if (*(commandLine + 1) == '"')
				{
					curValue.push_back('"');
					commandLine++;
				}
				else
					isInQueto = !isInQueto;
			}
			else if(curChr == 0)
			{
				TinyAssert(!isInQueto);
				if (isInQueto)
					return false;
				_splitedCommandLine[curName] = curValue;
				stateMachine = Start;
			}
			else if(curChr == ' ' && !isInQueto)
			{
				_splitedCommandLine[curName] = curValue;
				stateMachine = Start;
			}
			else
			{
				curValue.push_back(curChr);
			}
			break;
		}
		++commandLine;
	} while (curChr != 0);
	return true;
}

bool CommandLineCfg::isValidNameChar(char chr) const
{
	return (chr >= 'a' && chr <= 'z')
		|| (chr >= 'A' && chr <= 'Z')
		|| (chr >= '0' && chr <= '9')
		|| chr == '_';
}
