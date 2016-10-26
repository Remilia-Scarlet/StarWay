#include <iostream>
#include <windows.h>

#include "CommonCompiler.h"



void showHelp()
{
	printf("Useage:\n");
	printf("\tCommonCompiler [/? | /config configFilePath]\n\n");
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
	MD5Update(&ctx, (unsigned char*)str.c_str(), (unsigned int)str.length());
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
	CommonCompiler compiler;
	if (!compiler.parseArg(argc, argv))
	{
		showHelp();
		system("pause");
		return 1;
	}
	
	if (!compiler.readConfigFile())
	{
		std::string msg = "Can't open json config file [";
		msg += compiler.getConfigFilePath().getOriginPath();
		msg += "]\n";
		printf(msg.c_str());
		system("pause");
		return 2;
	}

	if (!compiler.readConfigFile())
	{
		std::string msg = "Can't open json config file [";
		msg += compiler.getConfigFilePath().getOriginPath();
		msg += "]\n";
		printf(msg.c_str());
		system("pause");
		return 2;
	}

	if (!compiler.compile())
		return 3;
	/*
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
	FindClose(hFind);*/

	return 0;
}