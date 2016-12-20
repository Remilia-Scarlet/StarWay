#pragma once
#include <functional>

class WavefrontParser
{
public:
	WavefrontParser(const std::string& fileName);
	bool startParse();
	void registerCommand(const std::string& cmd, const std::function<void()>& callback);
public:
	std::string nextCommand();
	std::string nextParam();
protected:
	std::string nextToken(bool isCommand);
	bool isBlankChar(char chara);

	std::string _currentCommand;
	std::string _fileName;
	std::vector<char> _fileData;
	int _currentPos;
	std::map<std::string, std::function<void()> > _commandList;
};