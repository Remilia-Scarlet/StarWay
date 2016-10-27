#include <iostream>
#include <windows.h>

#include "CommonCompiler.h"
#include "Engine\Ash\CommonFunc.h"

void showHelp()
{
	DebugString("Useage:\n");
	DebugString("\tCommonCompiler [/? | /config configFilePath]\n\n");
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
			DebugString("unable to load advapi32.dll\n");
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
	DebugString("Common compiler start\n");

	CommonCompiler compiler;
	int result = compiler.run(argc, argv);
	switch (result)
	{
	case 1:
		showHelp();
		system("pause");
		break;
	case 2:
		DebugString("Can't parse json config file:\n[%s]\n", compiler.getConfigFilePath().c_str());
		system("pause");
		break;
	case 3:
		DebugString("Find loop define in json config file:\n[%s]\n", compiler.getConfigFilePath().c_str());
		system("pause");
		break;
	case 4:
		DebugString("Can't open log file:\n[%s]\n", compiler.getLogFilePath().c_str());
		system("pause");
		break;
	case 5:
		DebugString("Can't find source path:\n[%s]\n", compiler.getSourcePath().c_str());
		system("pause");
		break;
	default:
		break;
	}


	return 0;
}