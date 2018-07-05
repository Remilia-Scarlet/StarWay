#define _CRT_SECURE_NO_WARNINGS
#include "gtest/gtest.h"
#include "Ash/CommonFunc.h"
#include "Ash/CommandLineCfg/CommandLineCfg.h"
#include "Ash/FileSystem/fs_include.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"

TEST(Ash, StringSplitTest)
{
	std::string str = "1231231231233";

	{
		auto vec = split(str, "123");
		EXPECT_EQ(vec.size(), 1);
		EXPECT_EQ(vec.back(), "3");
	}
	//split(str, ""); //assert saying separator can not be empty
	{
		auto vec2 = split(str, "1");
		EXPECT_EQ(vec2.size(), 4);
		EXPECT_EQ(vec2[0], "23");
		EXPECT_EQ(vec2[1], "23");
		EXPECT_EQ(vec2[2], "23");
		EXPECT_EQ(vec2[3], "233");
	}
	{
		auto vec3 = split(str, "fdsa");
		EXPECT_EQ(vec3.size(), 1);
		EXPECT_EQ(vec3.back(), str);
	}
	{
		std::string str2 = "111111";
		auto vec4 = split(str2, "1");
		EXPECT_EQ(vec4.size(), 0);
	}
	{
		auto vec5 = split(str, "1231231231233");
		EXPECT_EQ(vec5.size(), 0);
	}
	{
		auto vec6 = split(str, "123123123123");
		EXPECT_EQ(vec6.size(), 1);
		EXPECT_EQ(vec6.back(), "3");
	}
	{
		auto vec7 = split(str, "3");
		EXPECT_EQ(vec7.size(), 4);
		EXPECT_EQ(vec7[0], "12");
		EXPECT_EQ(vec7[1], "12");
		EXPECT_EQ(vec7[2], "12");
		EXPECT_EQ(vec7[3], "12");
	}
}

TEST(Ash, ReplaceStringTest)
{
	{
		std::string str = "1231231231233";
		replaceSubstr(str, "123", "abcdef");
		EXPECT_EQ(str, "abcdefabcdefabcdefabcdef3");
	}
	{
		std::string str2 = "1231231231233";
		replaceSubstr(str2, "1", "ab");
		EXPECT_EQ(str2, "ab23ab23ab23ab233");
	}
	{
		std::string str3 = "1231231231233";
		replaceSubstr(str3, "3", "ab");
		EXPECT_EQ(str3, "12ab12ab12ab12abab");
	}
	{
		std::string str4 = "";
		replaceSubstr(str4, "123", "ab");
		EXPECT_EQ(str4, "");
	}
	{
		std::string str5 = "1231231231233";
		replaceSubstr(str5, "", "ab");
		EXPECT_EQ(str5, "1231231231233");
	}
	{
		std::string str6 = "1231231231233";
		replaceSubstr(str6, "123", "");
		EXPECT_EQ(str6, "3");
	}
}
TEST(Ash, FormatStringTest)
{
	std::string s = FormatString("waa%s,%d", "233", 456);
	EXPECT_EQ(s, "waa233,456");
}

TEST(Ash, CharToWCharTest)
{
	std::wstring s = charToWchar("asb");
	EXPECT_EQ(s, L"asb");

	s = charToWchar("");
	EXPECT_EQ(s, L"");
}

TEST(Ash, WcharToCharTest)
{
	std::string s = wcharToChar(L"asb");
	EXPECT_EQ(s, "asb");

	s = wcharToChar(L"");
	EXPECT_EQ(s, "");
}

TEST(Ash, ToLowerTest)
{
	std::string s = "AfsdV#% ,\n\t123";
	std::string s1 = toLower(s);
	EXPECT_EQ(s1, "afsdv#% ,\n\t123");
}

TEST(Ash, ToUpperTest)
{
	std::string s = "AfsdV#% ,\n\t123";
	std::string s1 = toUpper(s);
	EXPECT_EQ(s1, "AFSDV#% ,\n\t123");
}

TEST(Ash, CommonCommandLineTest)
{
	class CommandLineCfgTest : public CommandLineCfg
	{
	public:
		DEFINE_COMMANDLINE_BOOL(Renderdoc, "/Renderdoc:Enable renderdoc in-app api"); //Add your bool or int or string commandline option here.
		DEFINE_COMMANDLINE_STR(Name, "default name", "/Name:[str], Some hint");          //Add your bool or int or string commandline option here.
		DEFINE_COMMANDLINE_INT(Number, 0, "SomeDesc");
	};

	{
		CommandLineCfgTest cmdLine;
		cmdLine.init(" asdf fdsag \"fdas\" /Renderdoc /Name:233 /Number:244 /SomeArgThatNotDefined:good /SomeArgThatNotDefined2");
		EXPECT_EQ(cmdLine.getName(), "233");
		EXPECT_EQ(cmdLine.getNumber(), 244);
		EXPECT_EQ(cmdLine.getRenderdoc(), true);
		auto it = cmdLine.getSplitedCommandLine().find(toLower("SomeArgThatNotDefined"));
		EXPECT_NE(it, cmdLine.getSplitedCommandLine().end());
		EXPECT_EQ(it->second, "good");
		it = cmdLine.getSplitedCommandLine().find(toLower("SomeArgThatNotDefined2"));
		EXPECT_NE(it, cmdLine.getSplitedCommandLine().end());
		EXPECT_EQ(it->second, "");
	}
	{
		CommandLineCfgTest cmdLine;
		cmdLine.init("/Name:df\"f dg g \"fsda");
		EXPECT_EQ(cmdLine.getName(), "dff dg g fsda");
	}
}

TEST(Ash, PathTest)
{
	char* exePath = new char[255];
	GetModuleFileNameA(NULL, exePath, 255);
	char* fullPath = new char[255];
	_fullpath(fullPath, exePath, 255);
	char* exeFolder = new char[255];
	char* exeDriver = new char[20];
	char* exeName = new char[100];
	char* exeExt = new char[20];
	_splitpath(fullPath, exeDriver, exeFolder, exeName, exeExt);
	{
		Path path(exePath);
		EXPECT_FALSE(path.isDirectory());
		EXPECT_TRUE(path.isFile());
		EXPECT_TRUE(path.isValid());
		auto list = path.getFileList();
		EXPECT_EQ(list.size(), 0);
		auto re = path.getRelativePath();
		EXPECT_EQ(re.substr(0, 31) , "..\\..\\..\\output\\_temp\\unittest\\"); 
		auto ab = path.getAbsolutePath();
		EXPECT_EQ(ab, fullPath);
	}
	{
		Path path(exePath);
		Path pa = path.getParentDirectory();
		EXPECT_EQ(pa.getRelativePath(), "..\\..\\..\\output\\_temp\\unittest\\");
	}
	{
		Path path2("game:");
		EXPECT_TRUE(path2.isDirectory());
		EXPECT_FALSE(path2.isFile());
		EXPECT_TRUE(path2.isValid());
		auto list2 = path2.getFileList();
		EXPECT_GE(list2.size(), 0u);
		auto re  = path2.getRelativePath();
		EXPECT_EQ(re, "..\\..\\..\\output\\_temp\\unittest\\");
		auto ab = path2.getAbsolutePath();
		EXPECT_EQ(ab, std::string(exeDriver) + std::string(exeFolder));
		Path pa = path2.getParentDirectory();
		EXPECT_EQ(pa.getRelativePath(), "..\\..\\..\\output\\_temp\\");
	}

	{
		Path path3("game:/../..\\Tools/luac/src/../build/luac_32.exe");
		auto ab = path3.getAbsolutePath();
		EXPECT_EQ(ab.find("/"), std::string::npos);
		EXPECT_EQ(ab.find(".."), std::string::npos);
		EXPECT_EQ(ab.substr(ab.size() - 36), "\\output\\Tools\\luac\\build\\luac_32.exe");
		auto re = path3.getRelativePath();
		EXPECT_EQ(re, "..\\..\\..\\output\\Tools\\luac\\build\\luac_32.exe");
	}
	{
		Path path5("folder\\");
		auto re = path5.getRelativePath();
		EXPECT_EQ(re, "folder\\");
	}
	{
		Path pa;
		EXPECT_EQ(pa.getRootPath().getAbsolutePath(), std::string(exeDriver) + std::string(exeFolder));
	}
	{
		Path pa(fullPath);
		EXPECT_EQ(pa.getFileName(), std::string(exeName) + std::string(exeExt));
		EXPECT_EQ(pa.getFileNameWithoutExt(), exeName);
		EXPECT_EQ(std::string(".") + pa.getFileExtension(), exeExt);

		Path pa2("folder\\");
		EXPECT_EQ(pa2.getFileName(), "");
		EXPECT_EQ(pa2.getFileNameWithoutExt(), "");
		EXPECT_EQ(pa2.getFileExtension(), "");

		Path pa3("folder");
		EXPECT_EQ(pa3.getFileName(), "");
		EXPECT_EQ(pa3.getFileNameWithoutExt(), "");
		EXPECT_EQ(pa3.getFileExtension(), "");
	}
	delete[] fullPath;
	delete[] exePath;
	delete[] exeFolder;
	delete[] exeDriver;
	delete[] exeName;
	delete[] exeExt;
}

class A : public RefCountObj
{
public:
	A()
	{
		++s_instanceNum;
	}
	~A()
	{
		--s_instanceNum;
	}
	int getValue() { return _value; }
	void setValue(int value) { _value = value; }
	int _value = 233;
	static int s_instanceNum;
};
int A::s_instanceNum = 0;
class B : public A
{
public:
	RefCountPtr<B> _b;
};
TEST(Ash, SmartPtr)
{
	RefCountPtr<A> smartPtr = RefCountPtr<A>(new A());
	EXPECT_EQ(smartPtr.getStrongRefCount(), 1);
	EXPECT_EQ(smartPtr.getWeakRefCount(), 0);

	WeakRefPtr<A> weakPtr = smartPtr;
	EXPECT_EQ(smartPtr.getStrongRefCount(), 1);
	EXPECT_EQ(smartPtr.getWeakRefCount(), 1);

	RefCountPtr<A> smartPtr2 = RefCountPtr<A>(new A());
	smartPtr2 = nullptr; // should not have memory leak for m_counter
	EXPECT_EQ(A::s_instanceNum, 1);

	WeakRefPtr<A> weakPtr2 = std::move(weakPtr); // should not crash here
	EXPECT_EQ(weakPtr.getStrongRefCount(), 0);
	EXPECT_EQ(weakPtr.getWeakRefCount(), 0);
	EXPECT_EQ(weakPtr2.getStrongRefCount(), 1);
	EXPECT_EQ(weakPtr2.getWeakRefCount(), 1);

	RefCountPtr<A> nullA = RefCountPtr<B>(nullptr);// should not crash
	smartPtr = smartPtr.get();			// should not crash
	RefCountPtr<B> bPtr = RefCountPtr<B>(new B);
	bPtr->_b = RefCountPtr<B>(new B);	// should not crash
	bPtr = bPtr->_b;					// should not crash
	EXPECT_EQ(bPtr->getValue(), 233);	// should not crash
	bPtr.reset();
	{
		RefCountPtr<A> smartPtr2;
		smartPtr2 = smartPtr;
		EXPECT_EQ(smartPtr.getStrongRefCount(), 2);
		EXPECT_EQ(smartPtr.getWeakRefCount(), 1);

		WeakRefPtr<A> weakPtr2 = smartPtr;
		EXPECT_EQ(smartPtr.getStrongRefCount(), 2);
		EXPECT_EQ(smartPtr.getWeakRefCount(), 2);
		RefCountPtr<A> lockPtr = weakPtr2.lock(); //should not crash here
	}
	EXPECT_EQ(smartPtr.getStrongRefCount(), 1);
	EXPECT_EQ(smartPtr.getWeakRefCount(), 1);
	smartPtr->setValue(123);
	int value = smartPtr->getValue();
	EXPECT_EQ(value, 123);

	smartPtr = nullptr;
	EXPECT_EQ(A::s_instanceNum, 0);
	void* ptr = smartPtr.get();
	EXPECT_EQ(ptr, nullptr);
	EXPECT_EQ(weakPtr2.getStrongRefCount(), 0);
	EXPECT_EQ(weakPtr2.getWeakRefCount(), 1);
}