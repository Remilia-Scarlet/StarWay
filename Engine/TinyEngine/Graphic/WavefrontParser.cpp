#include "TinyEngine/precomp.h"
#include "TinyEngine/Graphic/WavefrontParser.h"
#include "Ash/FileSystem/File_Win.h"

WavefrontParser::WavefrontParser(const std::string& fileName)
	: _fileName(fileName)
	, _currentPos(0)
{

}

bool WavefrontParser::startParse()
{
	File file;
	bool result = file.open(_fileName, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	if (!result)
	{
		DebugString("Can't open %s for wavefront parseing", _fileName.c_str());
		return false;
	}
	_fileData = file.readAll();
	file.close();

	auto itEnd = _commandList.end();
	auto it = itEnd;
	std::string currentCommand;
	while (true)
	{
		_currentCommand = nextCommand();
		if (_currentCommand.length() == 0)
			break;

		it = _commandList.find(_currentCommand);
		if (it != itEnd)
			(it->second)();
	};
	return true;
}

void WavefrontParser::registerCommand(const std::string& cmd, const std::function<void()>& callback)
{
	_commandList[cmd] = callback;
}

std::string WavefrontParser::nextCommand()
{
	return nextToken(true);
}

std::string WavefrontParser::nextParam()
{
	return nextToken(false);
}

std::string WavefrontParser::nextToken(bool isCommand)
{
	std::string token;
	int size = (int)_fileData.size();

	// if we need next command, skip all character until '\n' apears
	if (isCommand && !_currentCommand.empty())
	{
		while (_currentPos < size && _fileData[_currentPos] != '\n')
		{
			++_currentPos;
		}
	}

	while (true)
	{
		if (_currentPos == size)
			return token;
		char currentChar = _fileData[_currentPos];
		if (currentChar == '#')
		{
			if (token.length() > 0)
				break;
			do
			{
				++_currentPos;
			} while (_currentPos < size && _fileData[_currentPos] != '\n');
			if (_currentPos < size)
				++_currentPos;//skip the last '\n'
		}
		else if (isBlankChar(currentChar))
		{
			if (token.size() > 0)
				return token;
			++_currentPos;
			while (_currentPos < size && isBlankChar(_fileData[_currentPos]))
			{
				++_currentPos;
			}
		}
		else if (_fileData[_currentPos] == '\n')
		{
			if (!isCommand || (isCommand && token.size() > 0) )
				return token;
			++_currentPos;
		}
		else
		{
			token.push_back(currentChar);
			++_currentPos;
		}
	}
	return token;
}
bool WavefrontParser::isBlankChar(char chara)
{
	return chara == ' ' || chara == '\t' || chara == '\r';
}