#include "gtest/gtest.h"

TEST(Ash, StringSplitTest)
{
	std::string str = "1231231231233";

	{
		auto vec = Ash::split(str, "123");
		EXPECT_EQ(vec.size(), 1);
		EXPECT_EQ(vec.back(), "3");
	}
	{
		//EXPECT_THROW(split(str, "")); //assert saying separator can not be empty
	}
	{
		auto vec2 = Ash::split(str, "1");
		EXPECT_EQ(vec2.size(), 4);
		EXPECT_EQ(vec2[0], "23");
		EXPECT_EQ(vec2[1], "23");
		EXPECT_EQ(vec2[2], "23");
		EXPECT_EQ(vec2[3], "233");
	}
	{
		auto vec3 = Ash::split(str, "fdsa");
		EXPECT_EQ(vec3.size(), 1);
		EXPECT_EQ(vec3.back(), str);
	}
	{
		std::string str2 = "111111";
		auto vec4 = Ash::split(str2, "1");
		EXPECT_EQ(vec4.size(), 0);
	}
	{
		auto vec5 = Ash::split(str, "1231231231233");
		EXPECT_EQ(vec5.size(), 0);
	}
	{
		auto vec6 = Ash::split(str, "123123123123");
		EXPECT_EQ(vec6.size(), 1);
		EXPECT_EQ(vec6.back(), "3");
	}
	{
		auto vec7 = Ash::split(str, "3");
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
        Ash::replaceSubstr(str, "123", "abcdef");
		EXPECT_EQ(str, "abcdefabcdefabcdefabcdef3");
	}
	{
		std::string str2 = "1231231231233";
        Ash::replaceSubstr(str2, "1", "ab");
		EXPECT_EQ(str2, "ab23ab23ab23ab233");
	}
	{
		std::string str3 = "1231231231233";
        Ash::replaceSubstr(str3, "3", "ab");
		EXPECT_EQ(str3, "12ab12ab12ab12abab");
	}
	{
		std::string str4 = "";
        Ash::replaceSubstr(str4, "123", "ab");
		EXPECT_EQ(str4, "");
	}
	{
		std::string str5 = "1231231231233";
        Ash::replaceSubstr(str5, "", "ab");
		EXPECT_EQ(str5, "1231231231233");
	}
	{
		std::string str6 = "1231231231233";
        Ash::replaceSubstr(str6, "123", "");
		EXPECT_EQ(str6, "3");
	}
}
TEST(Ash, FormatStringTest)
{
	std::string s = Ash::FormatString("waa%s,%d", "233", 456);
	EXPECT_EQ(s, "waa233,456");
}

TEST(Ash, CharToWCharTest)
{
	std::wstring s = Ash::charToWchar("asb");
	EXPECT_EQ(s, L"asb");

	s = Ash::charToWchar("");
	EXPECT_EQ(s, L"");
}

TEST(Ash, WcharToCharTest)
{
	std::string s = Ash::wcharToChar(L"asb");
	EXPECT_EQ(s, "asb");

	s = Ash::wcharToChar(L"");
	EXPECT_EQ(s, "");
}

TEST(Ash, ToLowerTest)
{
	std::string s = "AfsdV#% ,\n\t123";
	std::string s1 = Ash::toLower(s);
	EXPECT_EQ(s1, "afsdv#% ,\n\t123");
}

TEST(Ash, ToUpperTest)
{
	std::string s = "AfsdV#% ,\n\t123";
	std::string s1 = Ash::toUpper(s);
	EXPECT_EQ(s1, "AFSDV#% ,\n\t123");
}
