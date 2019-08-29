#include "AshCore.h"
#include "CommandLineCfg.h"
#include "Ash/CommonFunc.h"
#include "Ash/TinyAssert.h"
#include <cctype>
#include "Ash/CommonStateMachine/StateMachine.h"

bool CommandLineCfg::init(const char* commandLine)
{
	bool result = spliteCommandLine(commandLine);
	if (!result)
		return false;
	return _initImp();
}

std::string CommandLineCfg::generateHelpString() const
{
	std::string ret;
	for (const auto& pa : _registedCommandLines)
	{
		ret += pa.second._description;
		ret += "\n";
	}
	return ret;
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
	StateMachine<char> sm;
	State<char>& start = sm.createState("start");
	State<char>& name = sm.createState("name");
	State<char>& value = sm.createState("value");

	start.DefineChangeTo(name, [](char chr){return chr == '/'; });

	class NameUserData : public StateUserData
	{
	public:
		std::string _name;
	};
	name.setUserData(std::make_shared<NameUserData>());
	name.onEnter([&name](State<char>&){
		name.getUserData<NameUserData>()._name.clear();
	});
	name.DefineChangeTo(value, [](char chr) {return chr == ':'; });
	name.DefineChangeTo(start
		,[](char chr) {return chr == ' ' || chr == 0; }
		, [this,&name](char chr){
			const std::string& curName = name.getUserData<NameUserData>()._name;
			if(!curName.empty())
				_splitedCommandLine.insert(std::pair<std::string, std::string>(curName, ""));
		});
	name.DefineHandleFun([&name](char chr){
		std::string& curName = name.getUserData<NameUserData>()._name;
		if ((chr >= 'a' && chr <= 'z')
			|| (chr >= 'A' && chr <= 'Z')
			|| (chr >= '0' && chr <= '9')
			|| chr == '_')
		{
			curName.push_back(std::tolower(chr));
		}
		else
		{
			TinyAssert(false, "Parsing command line failed");
		}
	});
	struct ValueStateUserData : public StateUserData
	{
		std::string curValue;
		bool isInQueto = false;
		bool lastIsQueto = false;
	};
	value.setUserData(std::make_shared<ValueStateUserData>());
	value.onEnter([&value](State<char>&)
	{
		value.getUserData<ValueStateUserData>().curValue.clear();
		value.getUserData<ValueStateUserData>().isInQueto = false;
		value.getUserData<ValueStateUserData>().lastIsQueto = false;
	});
	value.DefineChangeTo(start, [&value](char chr){
		bool& isInQueto = value.getUserData<ValueStateUserData>().isInQueto;
		bool& lastIsQueto = value.getUserData<ValueStateUserData>().lastIsQueto;
		if(chr == 0)
		{
			TinyAssert(!isInQueto);
			return true;
		}
		else if((chr == ' ' && !isInQueto)
			|| (chr == ' ' && isInQueto && lastIsQueto))
		{
			return true;
		}
		return false;
	}
	,[&name,&value,this](char chr){
		std::string& curName = name.getUserData<NameUserData>()._name;
		std::string& curValue = value.getUserData<ValueStateUserData>().curValue;
		_splitedCommandLine.insert(std::pair<std::string,std::string>(curName, curValue));
	});
	value.DefineHandleFun([&value](char chr)
	{
		std::string& curValue = value.getUserData<ValueStateUserData>().curValue;
		bool& isInQueto = value.getUserData<ValueStateUserData>().isInQueto;
		bool& lastIsQueto = value.getUserData<ValueStateUserData>().lastIsQueto;
		if (chr == '"')
		{
			if (lastIsQueto)
			{
				curValue.push_back('"');
				lastIsQueto = false;
			}
			else
				lastIsQueto = true;
		}
		else
		{
			if(lastIsQueto)
			{
				isInQueto = !isInQueto;
				lastIsQueto = false;
			}
			curValue.push_back(chr);
		}
	});

	sm.run(start, [&commandLine]() -> std::optional<char>{
		if(commandLine == nullptr)
			return std::nullopt;
		char chr = *commandLine;
		if (chr == 0)
			commandLine = nullptr;
		else
			++commandLine;
		return std::optional<char>{chr};
	});
	return true;
}
