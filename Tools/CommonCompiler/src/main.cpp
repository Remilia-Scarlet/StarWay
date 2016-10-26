#include <iostream>
#include <windows.h>
#include <fstream>
#include "..\..\..\Engine\ThirdParty\rapidjson\document.h"
#include "..\..\..\Engine\ThirdParty\rapidjson\writer.h"
#include "..\..\..\Engine\ThirdParty\rapidjson\fwd.h"
#include "..\..\..\Engine\ThirdParty\rapidjson\stringbuffer.h"

using namespace rapidjson;

void showHelp()
{
	printf("Useage:\n");
	printf("\tCommonCompiler [/? | /config configFilePath]\n\n");
}

void errorArg()
{
	showHelp();
	system("pause");
}

bool parseArg(int argc, char* argv[], std::string& config)
{
	if (argc <= 1)
	{
		errorArg();
		return false;
	}

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "/?") == 0)
		{
			showHelp();
			return false;
		}
		else if (strcmp(argv[i], "/config") == 0)
		{
			if (i + 1 < argc)
			{
				config = argv[i + 1];
				return true;
			}
			else
			{
				errorArg();
				return false;
			}
		}
	}
	return false;
}

bool getFileData(const std::string& fileName, std::string& fileData)
{
	std::ifstream file(fileName);
	if (!file)
	{
		std::string msg = "Can't open file [";
		msg += fileName + "]\n";
		printf(msg.c_str());
		system("pause");
		return false;
	}
	file.seekg(0, std::ios::end);
	std::streampos len = file.tellg();
	file.seekg(0, std::ios::beg);
	char* data = new char[(int)len + 1];
	memset(data, 0, (int)len + 1);
	file.read(data, len);
	file.close();
	fileData = data;
	delete[] data;
	return true;
}

bool parseJson(const std::string& fileData, std::string& sourcepath, std::string& sourcefilter, std::string& tempfile, std::string& dependency, std::string& output, std::string& cmd)
{
	Document json;
	json.Parse(fileData.c_str());
	if (json.HasParseError())
		return false;

	if (json.HasMember("sourcepath"))
		sourcepath = json["sourcepath"].GetString();

	if (json.HasMember("sourcefilter"))
		sourcefilter = json["sourcefilter"].GetString();

	if (json.HasMember("tempfile"))
		tempfile = json["tempfile"].GetString();

	if (json.HasMember("dependency"))
		dependency = json["dependency"].GetString();

	if (json.HasMember("output"))
		output = json["output"].GetString();

	if (json.HasMember("cmd"))
		cmd = json["cmd"].GetString();
	return true;
}

std::string getMD5(const std::string& str)
{
	typedef struct
	{
		ULONG i[2];
		ULONG buff[4];
		unsigned char in[64];
		unsigned char digest[16];
	}MD5_CTX;

	static HINSTANCE hDll = NULL;

	typedef void(WINAPI* PMD5Init)(MD5_CTX*);
	typedef void(WINAPI* PMD5Update)(MD5_CTX*, const unsigned char*, unsigned int);
	typedef void(WINAPI* PMD5Final)(MD5_CTX*);
	static PMD5Init MD5Init;
	static PMD5Update MD5Update;
	static PMD5Final MD5Final;
	if (hDll == NULL)
	{
		hDll = LoadLibrary("advapi32.dll");
		if (hDll < 0)
		{
			printf("unable to load advapi32.dll\n");
			system("pause");
			exit(4);
		}
		MD5Init = (PMD5Init)GetProcAddress(hDll, "MD5Init");
		MD5Update = (PMD5Update)GetProcAddress(hDll, "MD5Update");
		MD5Final = (PMD5Final)GetProcAddress(hDll, "MD5Final");
	}

	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx, (unsigned char*)str.c_str(), str.length());
	MD5Final(&ctx);
	char ASC[32 + 1];
	ASC[32] = '\0';
	for (int i = 0; i < 16; i++)
	{
		ASC[i * 2] = "0123456789ABCDEF"[ctx.digest[i] >> 4];
		ASC[i * 2 + 1] = "0123456789ABCDEF"[ctx.digest[i] & 0x0F];
	}
	return ASC;
}

int main(int argc, char* argv[])
{
	std::string config;
	if (!parseArg(argc, argv, config))
		return 1;
	
	std::string fileData;
	if (!getFileData(config, fileData))
		return 2;

	std::string sourcepath, sourcefilter, tempfile, dependency, output, cmd;
	if (!parseJson(fileData, sourcepath, sourcefilter, tempfile, dependency, output, cmd))
		return 3;

	Document log;
	log.Parse(tempfile.c_str());
	if (log.HasParseError())
	{
		std::ofstream logfile(tempfile, std::ios::out | std::ios::trunc);
		if (!logfile)
		{
			std::string msg = "Can't open ";
			msg += tempfile + "\n";
			printf(msg.c_str());
			system("pause");
			exit(5);
		}
		StringBuffer strbuffer;
		Writer<StringBuffer> writer(strbuffer);
		log.Clear();
		log.Accept(writer);

	}

	WIN32_FIND_DATA att;
	HANDLE hFind = FindFirstFile((sourcepath + sourcefilter).c_str(),&att);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	do 
	{
		if (att.cFileName[0] != '.')
		{
			if (att.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				
			}
			else
			{
				std::string filename = sourcepath + att.cFileName;

			}
		}
	} while (::FindNextFile(hFind, &att));
	FindClose(hFind);

	return 0;
}