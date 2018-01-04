#pragma once

#include <map>
#include <functional>
#include <algorithm>
#include <vector>

class CommandLineCfg;

class CommandLineHelper
{
	friend class CommandLineCfg;
public:
	CommandLineHelper() = default;
	CommandLineHelper(std::string name, const char* description, std::function<bool(CommandLineCfg*, const std::string&)> settingFun)
		: _description(description)
		, _settingFun(settingFun)
	{
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		s_commandLines[name] = *this;
	}
	const char* _description = nullptr;
	std::function<bool(CommandLineCfg*, const std::string&)> _settingFun;
	static std::map<std::string, CommandLineHelper > s_commandLines;
};

class CommandLineCfg
{
	friend class CommandLineHelper;
public:
	static bool createInstance(const char* commandLine);
	static CommandLineCfg* instance();
	static void destroy();

#define DEFINE_COMMANDLINE(COMMAND,TYPE,DEFAULT_VALUE,DESCREPTION,SETTING_FUNCTION) protected: TYPE _##COMMAND = (CommandLineHelper(#COMMAND,DESCREPTION,SETTING_FUNCTION),DEFAULT_VALUE); public: TYPE get##COMMAND(){return _##COMMAND;}
#define DEFINE_COMMANDLINE_BOOL(COMMAND,DESCREPTION) DEFINE_COMMANDLINE(COMMAND,bool,false,DESCREPTION,[](CommandLineCfg* instance, const std::string&){ instance->_##COMMAND = true; return true; })
#define DEFINE_COMMANDLINE_INT(COMMAND,DEFAULT_VALUE,DESCREPTION) DEFINE_COMMANDLINE(COMMAND,int,DEFAULT_VALUE,DESCREPTION,[](CommandLineCfg* instance, const std::string& value){ instance->_##COMMAND = std::stoi(value); return true; })
#define DEFINE_COMMANDLINE_STR(COMMAND,DEFAULT_VALUE,DESCREPTION) DEFINE_COMMANDLINE(COMMAND,std::string,DEFAULT_VALUE,DESCREPTION,[](CommandLineCfg* instance, const std::string& value){ instance->_##COMMAND = value; return true; })
//-----------------------------------Commandlines define here------------------------------
	DEFINE_COMMANDLINE_BOOL(Renderdoc, "Enable renderdoc in-app api")

//-----------------------------------End commandlines define------------------------------
#undef DEFINE_COMMANDLINE_BOOL
#undef DEFINE_COMMANDLINE_INT
#undef DEFINE_COMMANDLINE_STR
#undef DEFINE_COMMANDLINE

protected:
	CommandLineCfg() = default;
	~CommandLineCfg() = default;
	bool init(CommandLineCfg* instance, const char* commandLine);
	std::vector<std::string> spliteCommandLine(const char* commandLine);
protected:
	static CommandLineCfg* s_instance;


};