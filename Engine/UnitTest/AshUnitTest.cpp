#include "precomp.h"
#define _CRT_SECURE_NO_WARNINGS
#include "gtest/gtest.h"
#include "Ash/CommonFunc.h"
#include "Ash/CommandLineCfg/CommandLineCfg.h"
#include "Ash/FileSystem/fs_include.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Ash/Container/RingBuffer.h"
#include "Ash/MultiThread/Task.h"
#include "Ash/MultiThread/ThreadPool.h"
#include <algorithm>

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

class TestRingBufferClass
{
public:
	TestRingBufferClass(char ch, int count) : _data(count,ch){ ++s_instanceNumber; }
	TestRingBufferClass(std::string para) :_data(para) { ++s_instanceNumber; }
	TestRingBufferClass(const TestRingBufferClass& other) :_data(other._data) { ++s_instanceNumber; }
	TestRingBufferClass(TestRingBufferClass&&) = default;
	TestRingBufferClass& operator= (const TestRingBufferClass&) = delete;
	TestRingBufferClass& operator= (TestRingBufferClass&&) = default;
	~TestRingBufferClass() { if (!_data.empty()) --s_instanceNumber; }
	std::string _data;

	static int s_instanceNumber;
};
int TestRingBufferClass::s_instanceNumber = 0;

TEST(Ash, RingBufferTest)
{
	{
		RingBuffer<int> r1;
		EXPECT_EQ(r1.getSize(), 0);
		EXPECT_EQ(r1.getCapacity(), 0);

		r1.setCapacity(0);
		EXPECT_EQ(r1.getCapacity(), 0);

		r1.setCapacity(3);
		EXPECT_EQ(r1.getCapacity(), 3);
		EXPECT_EQ(r1.getSize(), 0);

		r1.emplaceBackItem(123);
		EXPECT_EQ(r1.back(), 123);
		EXPECT_EQ(r1.front(), 123);
		EXPECT_EQ(r1.getSize(), 1);
		EXPECT_EQ(r1.getFreeCount(), 2);

		r1.emplaceFrontItem(456);
		EXPECT_EQ(r1.back(), 123);
		EXPECT_EQ(r1.front(), 456);
		EXPECT_EQ(r1.getSize(), 2);
		EXPECT_EQ(r1.getFreeCount(), 1);
		EXPECT_EQ(r1.isFull(), false);

		r1.emplaceBackItem(789);
		EXPECT_EQ(r1.back(), 789);
		EXPECT_EQ(r1.front(), 456);
		EXPECT_EQ(r1.getSize(), 3);
		EXPECT_EQ(r1.getFreeCount(), 0);
		EXPECT_EQ(r1.isFull(), true);

		//EXPECT_DEBUG_DEATH(r1.emplaceFrontItem(0), "");
	}
	{
		RingBuffer<int> r3{ 1,2,3 };

	}
	{
		RingBuffer<int> r1(3);
		EXPECT_EQ(r1.getSize(), 0);
		EXPECT_EQ(r1.getCapacity(), 3);

		r1.setCapacity(0);
		EXPECT_EQ(r1.getCapacity(), 3);

		r1.setCapacity(3);
		EXPECT_EQ(r1.getCapacity(), 3);
		EXPECT_EQ(r1.getSize(), 0);

		r1.emplaceBackItem(123);
		EXPECT_EQ(r1.back(), 123);
		EXPECT_EQ(r1.front(), 123);
		EXPECT_EQ(r1.getSize(), 1);
		EXPECT_EQ(r1.getFreeCount(), 2);

		r1.emplaceFrontItem(456);
		EXPECT_EQ(r1.back(), 123);
		EXPECT_EQ(r1.front(), 456);
		EXPECT_EQ(r1.getSize(), 2);
		EXPECT_EQ(r1.getFreeCount(), 1);
		EXPECT_EQ(r1.isFull(), false);

		r1.popBack();
		EXPECT_EQ(r1.back(), 456);
		EXPECT_EQ(r1.front(), 456);
		EXPECT_EQ(r1.getSize(), 1);
		EXPECT_EQ(r1.getFreeCount(), 2);

		r1.emplaceBackItem(789);
		EXPECT_EQ(r1.back(), 789);
		EXPECT_EQ(r1.front(), 456);
		EXPECT_EQ(r1.getSize(), 2);
		EXPECT_EQ(r1.getFreeCount(),1);
		EXPECT_EQ(r1.isFull(), false);

		r1.emplaceFrontItem(0);
		EXPECT_EQ(r1.back(), 789);
		EXPECT_EQ(r1.front(), 0);
		EXPECT_EQ(r1.getSize(), 3);
		EXPECT_EQ(r1.getFreeCount(), 0);
		EXPECT_EQ(r1.isFull(), true);

		r1.reset();
		EXPECT_EQ(r1.getSize(), 0);
		EXPECT_EQ(r1.getFreeCount(), 0);
		EXPECT_EQ(r1.getCapacity(), 0);
	}
	{
		RingBuffer<TestRingBufferClass> rr;
		int instanceNum;
		{
			RingBuffer<TestRingBufferClass> r1;

			int index = 100000;
			while (--index)
			{
				int opera = rand() % 9;
				switch (opera)
				{
				case 0:
					if (r1.getFreeCount() == 0)
						r1.setCapacity(r1.getCapacity() + 1);
					r1.emplaceBackItem("str");
					EXPECT_EQ(r1.getSize(), TestRingBufferClass::s_instanceNumber);
					break;
				case 1:
				{
					int elemNum = rand() % 10;
					if (r1.getFreeCount() < elemNum)
						r1.setCapacity(r1.getCapacity() + elemNum);
					r1.emplaceBackItems(elemNum, 'a', 10);
					EXPECT_EQ(r1.getSize(), TestRingBufferClass::s_instanceNumber);
				}
				break;
				case 2:
					if (r1.getFreeCount() == 0)
						r1.setCapacity(r1.getCapacity() + 1);
					r1.emplaceFrontItem("strf");
					EXPECT_EQ(r1.getSize(), TestRingBufferClass::s_instanceNumber);
					break;
				case 3:
				{
					int elemNum = rand() % 10;
					if (r1.getFreeCount() < elemNum)
						r1.setCapacity(r1.getCapacity() + elemNum);
					r1.emplaceFrontItems(elemNum, 'b', 12);
					EXPECT_EQ(r1.getSize(), TestRingBufferClass::s_instanceNumber);
				}
				case 4:
					if (r1.getFreeCount() > 0)
					{
						EXPECT_NO_THROW(r1.front());
					}
					break;
				case 5:
					if (r1.getFreeCount() > 0)
					{
						EXPECT_NO_THROW(r1.popFront());
						EXPECT_EQ(r1.getSize(), TestRingBufferClass::s_instanceNumber);
					}
					break;
				case 6:
					if (r1.getFreeCount() > 0)
					{
						EXPECT_NO_THROW(r1.back());
					}
					break;
				case 7:
					if (r1.getFreeCount() > 0)
					{
						EXPECT_NO_THROW(r1.popBack());
						EXPECT_EQ(r1.getSize(), TestRingBufferClass::s_instanceNumber);
					}
					break;
				case 8:
					r1.reset();
					EXPECT_EQ(r1.getSize(), TestRingBufferClass::s_instanceNumber);
					break;
				}
			}

			instanceNum = TestRingBufferClass::s_instanceNumber;
			rr = r1;
			int instanceNum2 = TestRingBufferClass::s_instanceNumber;
			EXPECT_EQ(instanceNum * 2, instanceNum2);
		}
		{
			EXPECT_EQ(TestRingBufferClass::s_instanceNumber, instanceNum);
			RingBuffer<TestRingBufferClass> rr2{ std::move(rr) };
			EXPECT_EQ(rr.getSize(), 0);
			EXPECT_EQ(rr.getCapacity(), 0);
			EXPECT_EQ(TestRingBufferClass::s_instanceNumber, instanceNum);

			RingBuffer<TestRingBufferClass> rr3 { std::string("abc"),std::string("2"),std::string("3") };
			EXPECT_EQ(rr3.getSize(), 3);
			EXPECT_EQ(rr3.getCapacity(), 3);
			int ii = 0;
			RingBuffer<TestRingBufferClass>::iterator it = rr3.begin();
			for(;it != rr3.end();++it)
			{
				switch(ii)
				{
				case 0:
					EXPECT_EQ(it->_data, "abc");
					break;
				case 1:
					EXPECT_EQ((*it)._data, "2");
					break;
				case 2:
					EXPECT_EQ(it->_data, "3");
					break;
				default:
					ADD_FAILURE(); //should not have more
				}
				++ii;
			}
			it = rr3.begin();
			EXPECT_EQ(it->_data, "abc");
			EXPECT_EQ((it++)->_data, "abc");;
			EXPECT_EQ(it->_data, "2");
			EXPECT_EQ((++it)->_data, "3");;
			EXPECT_EQ(it->_data, "3");
			EXPECT_EQ((--it)->_data, "2");
			EXPECT_EQ(it->_data, "2");
			EXPECT_EQ((it--)->_data, "2");

			RingBuffer<int> rr4 { 0,1,2 };
			RingBuffer<int>::ringIterator itt{ rr4.ringBegin() };
			for(int iii = 0 ; iii < 5; ++iii)
			{
				EXPECT_EQ(*itt, iii % 3);
				if (iii % 2)
				{
					EXPECT_EQ(*(itt++), iii % 3);
				}
				else
				{
					EXPECT_EQ(*(++itt), (iii+1) % 3);
				}
			}
			itt = rr4.ringBegin();
			for (int iii = 0; iii > -5; --iii)
			{
				EXPECT_EQ(*itt, (iii+6) % 3);
				if (iii % 2)
				{
					EXPECT_EQ(*(itt--), (iii + 6) % 3);
				}
				else
				{
					EXPECT_EQ(*(--itt), (iii - 1 + 6) % 3);
				}
			}

			rr3 = std::move(rr2);
			EXPECT_EQ(TestRingBufferClass::s_instanceNumber, instanceNum);
		}
	}
	EXPECT_EQ(TestRingBufferClass::s_instanceNumber, 0);
}

TEST(Ash, ThreadPoolTest)
{
	constexpr int threadNumber = 12;
	std::vector<int> numbers(20000000);
	for (auto& i : numbers)
	{
		i = (rand() & 0xfff) << 12 | (rand() & 0xfff);
	}

	std::less<> _Pred;

	auto _Move_backward_unchecked = [](int* _First, int* _Last, int* _Dest) -> int*
	{	// move [_First, _Last) backwards to [..., _Dest), choose optimization
		// note: _Move_backward_unchecked is called directly from elsewhere in the STL
		while (_First != _Last)
			*--_Dest = _STD move(*--_Last);
		return (_Dest);
	};


	// FUNCTION TEMPLATE sort
	auto _Insertion_sort_unchecked = [&](int* _First, int* _Last) -> int*
	{	// insertion sort [_First, _Last), using _Pred
		if (_First != _Last)
		{
			for (int* _Next = _First; ++_Next != _Last; )
			{	// order next element
				int* _Next1 = _Next;
				int _Val = _STD move(*_Next);

				if (_Pred( _Val, *_First))
				{	// found new earliest element, move to front
					_Move_backward_unchecked(_First, _Next, ++_Next1);
					*_First = _STD move(_Val);
				}
				else
				{	// look for insertion point after first
					for (int* _First1 = _Next1;
						_Pred( _Val, *--_First1);
						_Next1 = _First1)
					{
						*_Next1 = _STD move(*_First1);	// move hole down
					}

					*_Next1 = _STD move(_Val);	// insert element in hole
				}
			}
		}

		return (_Last);
	};

	auto _Med3_unchecked = [&](int* _First, int* _Mid, int* _Last)
	{	// sort median of three elements to middle
		if (_Pred( *_Mid, *_First))
		{
			_STD iter_swap(_Mid, _First);
		}

		if (_Pred( *_Last, *_Mid))
		{	// swap middle and last, then test first again
			_STD iter_swap(_Last, _Mid);

			if (_Pred( *_Mid, *_First))
			{
				_STD iter_swap(_Mid, _First);
			}
		}
	};

	auto _Guess_median_unchecked = [&](int* _First, int* _Mid, int* _Last)
	{	// sort median element to middle
		using _Diff = std::_Iter_diff_t<int*>;
		const _Diff _Count = _Last - _First;
		if (40 < _Count)
		{	// median of nine
			const _Diff _Step = (_Count + 1) >> 3; // +1 can't overflow because range was made inclusive in caller
			const _Diff _Two_step = _Step << 1; // note: intentionally discards low-order bit
			_Med3_unchecked(_First, _First + _Step, _First + _Two_step);
			_Med3_unchecked(_Mid - _Step, _Mid, _Mid + _Step);
			_Med3_unchecked(_Last - _Two_step, _Last - _Step, _Last);
			_Med3_unchecked(_First + _Step, _Mid, _Last - _Step);
		}
		else
		{
			_Med3_unchecked(_First, _Mid, _Last);
		}
	};

	typedef int* _RanIt;
	auto _Partition_by_median_guess_unchecked = [&](_RanIt _First, _RanIt _Last) -> std::pair<_RanIt, _RanIt>
	{	// partition [_First, _Last), using _Pred
		_RanIt _Mid = _First + ((_Last - _First) >> 1);	// TRANSITION, VSO#433486
		_Guess_median_unchecked(_First, _Mid, _Last - 1);
		_RanIt _Pfirst = _Mid;
		_RanIt _Plast = _Pfirst + 1;

		while (_First < _Pfirst
			&& !_Pred( *(_Pfirst - 1), *_Pfirst)
			&& !_Pred(*_Pfirst, *(_Pfirst - 1)))
		{
			--_Pfirst;
		}

		while (_Plast < _Last
			&& !_Pred( *_Plast, *_Pfirst)
			&& !_Pred(*_Pfirst, *_Plast))
		{
			++_Plast;
		}

		_RanIt _Gfirst = _Plast;
		_RanIt _Glast = _Pfirst;

		for (;;)
		{	// partition
			for (; _Gfirst < _Last; ++_Gfirst)
			{
				if (_Pred( *_Pfirst, *_Gfirst))
				{
				}
				else if (_Pred(*_Gfirst, *_Pfirst))
				{
					break;
				}
				else if (_Plast != _Gfirst)
				{
					_STD iter_swap(_Plast, _Gfirst);
					++_Plast;
				}
				else
				{
					++_Plast;
				}
			}

			for (; _First < _Glast; --_Glast)
			{
				if (_Pred( *(_Glast - 1), *_Pfirst))
				{
				}
				else if (_Pred(*_Pfirst, *(_Glast - 1)))
				{
					break;
				}
				else if (--_Pfirst != _Glast - 1)
				{
					_STD iter_swap(_Pfirst, _Glast - 1);
				}
			}

			if (_Glast == _First && _Gfirst == _Last)
			{
				return (std::pair<_RanIt, _RanIt>(_Pfirst, _Plast));
			}

			if (_Glast == _First)
			{	// no room at bottom, rotate pivot upward
				if (_Plast != _Gfirst)
				{
					_STD iter_swap(_Pfirst, _Plast);
				}

				++_Plast;
				_STD iter_swap(_Pfirst, _Gfirst);
				++_Pfirst;
				++_Gfirst;
			}
			else if (_Gfirst == _Last)
			{	// no room at top, rotate pivot downward
				if (--_Glast != --_Pfirst)
				{
					_STD iter_swap(_Glast, _Pfirst);
				}

				_STD iter_swap(_Pfirst, --_Plast);
			}
			else
			{
				_STD iter_swap(_Gfirst, --_Glast);
				++_Gfirst;
			}
		}
	};

	std::function<void(_RanIt, _RanIt, std::_Iter_diff_t<_RanIt>)> _Sort_unchecked = [&](_RanIt _First, _RanIt _Last,std::_Iter_diff_t<_RanIt> _Ideal)
	{	// order [_First, _Last), using _Pred
		std::_Iter_diff_t<_RanIt> _Count;
		while (32 < (_Count = _Last - _First) && 0 < _Ideal)
		{	// divide and conquer by quicksort
			auto _Mid = _Partition_by_median_guess_unchecked(_First, _Last);
			// TRANSITION, VSO#433486
			_Ideal = (_Ideal >> 1) + (_Ideal >> 2);	// allow 1.5 log2(N) divisions

			if (_Mid.first - _First < _Last - _Mid.second)
			{	// loop on second half
				_Sort_unchecked(_First, _Mid.first, _Ideal);
				_First = _Mid.second;
			}
			else
			{	// loop on first half
				_Sort_unchecked(_Mid.second, _Last, _Ideal);
				_Last = _Mid.first;
			}
		}

		if (32 < _Count)
		{	// heap sort if too many divisions
			_Make_heap_unchecked(_First, _Last, _Pred);
			_Sort_heap_unchecked(_First, _Last, _Pred);
		}
		else if (2 <= _Count)
		{
			_Insertion_sort_unchecked(_First, _Last);	// small
		}
	};

	auto quickSort = [&](const _RanIt _First, const _RanIt _Last)
	{	// order [_First, _Last), using _Pred
		_Sort_unchecked(_First, _Last, _Last - _First);
	};

	//MultiThread quick sort
	{
		std::vector<int> data = numbers;
		auto start = std::chrono::system_clock::now();
		{
			quickSort(data.data(), data.data() + data.size());
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		DebugString("MultiThread cost: %f seconds", diff.count());
		bool orderIsRight = true;
		for (int i = 1; i < (int)data.size(); ++i)
		{
			if (data[i] < data[i - 1])
			{
				orderIsRight = false;
				break;
			}
		}
		EXPECT_TRUE(orderIsRight);
	}

	//std::sort
	{
		auto start = std::chrono::system_clock::now();
		{
			std::sort(numbers.begin(), numbers.end(), std::less<int>());
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		DebugString("std::sort cost: %f seconds", diff.count());
	}
}