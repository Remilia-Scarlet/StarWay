#pragma once

class WavefrontObjReader
{
public:
	static WavefrontObjReader* instance();
public:
	template<class T>
	bool readObjFile(const std::string& fileName,std::vector<T>& outVetexs,Material);
protected:

};