#pragma once
#include <list>

static const char* GAME_PATH = "game:";

class Path
{
public:
	Path();
	Path(const std::string& path);
	Path(const char* path);

	bool isDirectory() const;
	bool isFile() const;

	std::list<Path> getSubFile() const;
	const std::string& getResolvedPath() const;
protected:
	std::string _path;
	mutable std::string _resolvedPath;

	void _getSubFile(std::list<Path>& list) const;
};