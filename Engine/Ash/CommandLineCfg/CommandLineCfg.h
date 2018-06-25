#pragma once

#include <map>
#include <functional>
#include <algorithm>
#include <vector>
#include <locale>

class CommandLineCfg;

class CommandLineHelper
{
	friend class CommandLineCfg;
public:
	CommandLineHelper() = default;
	CommandLineHelper(std::map<std::string, CommandLineHelper>& registedCommandLines,std::string name, const char* description, std::function<bool(const std::string&)> settingFun)
		: _description(description)
		, _settingFun(settingFun)
	{
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		registedCommandLines[name] = *this;
	}
	const char* _description = nullptr;
	std::function<bool(const std::string&)> _settingFun;
};

#define DEFINE_COMMANDLINE(COMMAND,TYPE,DEFAULT_VALUE,DESCREPTION,SETTING_FUNCTION) protected: TYPE _##COMMAND = (CommandLineHelper(_registedCommandLines,#COMMAND,DESCREPTION,SETTING_FUNCTION),DEFAULT_VALUE); public: TYPE get##COMMAND(){return _##COMMAND;}
#define DEFINE_COMMANDLINE_BOOL(COMMAND,DESCREPTION) DEFINE_COMMANDLINE(COMMAND,bool,false,DESCREPTION, [this](const std::string&){ _##COMMAND = true; return true; })
#define DEFINE_COMMANDLINE_INT(COMMAND,DEFAULT_VALUE,DESCREPTION) DEFINE_COMMANDLINE(COMMAND,int,DEFAULT_VALUE,DESCREPTION,[this](const std::string& value){ _##COMMAND = std::stoi(value); return true; })
#define DEFINE_COMMANDLINE_STR(COMMAND,DEFAULT_VALUE,DESCREPTION) DEFINE_COMMANDLINE(COMMAND,std::string,DEFAULT_VALUE,DESCREPTION,[this](const std::string& value){ _##COMMAND = value; return true; })

/*
Useage : inherite your own cmdline class
For example:
 class MyCommandLineCfg : public CommandLineCfg
 {
 public:
    DEFINE_COMMANDLINE_BOOL(Renderdoc, "Enable renderdoc in-app api"); //Add your bool or int or string commandline option here.
    DEFINE_COMMANDLINE_STR(Name,"default name", "Some hint");          //Add your bool or int or string commandline option here.
 };

Then in some place:
 const char* commandline = "/Renderdoc /Name:abcd";
 MyCommandLineCfg cmdLine;
 cmdLine.init(commandline);
 bool renderdocEnable = cmdLine.getRenderdoc();
 std::string someName = cmdLine.getName();

*/
class CommandLineCfg
{
	friend class CommandLineHelper;
public:
	bool init(const char* commandLine);
	const std::map<std::string,std::string>& getSplitedCommandLine() const { return _splitedCommandLine; }
	std::string generateHelpString() const;
protected:
	bool _initImp();
	bool spliteCommandLine(const char* commandLine);
	std::map<std::string,std::string> _splitedCommandLine;
	std::map<std::string, CommandLineHelper> _registedCommandLines;
};