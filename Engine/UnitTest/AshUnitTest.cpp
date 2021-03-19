#include "precomp.h"
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <Windows.h>

#include "gtest/gtest.h"
#include "Ash/CommandLineCfg/CommandLineCfg.h"
#include "Ash/FileSystem/fs_include.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Ash/Container/RingBuffer.h"
#include "Ash/MultiThread/Task.h"
#include "Ash/MultiThread/ThreadPool.h"
#include "Ash/MultiThread/TaskRingBuffer.h"
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
	class CommandLineCfgTest : public Ash::CommandLineCfg
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
/*
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
		Ash::Path path(exePath);
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
		Ash::Path path(exePath);
		Ash::Path pa = path.getParentDirectory();
		EXPECT_EQ(pa.getRelativePath(), "..\\..\\..\\output\\_temp\\unittest\\");
	}
	{
		Ash::Path path2("game:");
		EXPECT_TRUE(path2.isDirectory());
		EXPECT_FALSE(path2.isFile());
		EXPECT_TRUE(path2.isValid());
		auto list2 = path2.getFileList();
		EXPECT_GE(list2.size(), 0u);
		auto re  = path2.getRelativePath();
		EXPECT_EQ(re, "..\\..\\..\\output\\_temp\\unittest\\");
		auto ab = path2.getAbsolutePath();
		EXPECT_EQ(ab, std::string(exeDriver) + std::string(exeFolder));
		Ash::Path pa = path2.getParentDirectory();
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
*/

class A : public Ash::RefCountObj
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
	Ash::RefCountPtr<B> _b;
};

TEST(Ash, SmartPtr)
{
	Ash::RefCountPtr<A> smartPtr = Ash::RefCountPtr<A>(new A());
	EXPECT_EQ(smartPtr.getStrongRefCount(), 1);
	EXPECT_EQ(smartPtr.getWeakRefCount(), 0);

	Ash::WeakRefPtr<A> weakPtr = smartPtr;
	EXPECT_EQ(smartPtr.getStrongRefCount(), 1);
	EXPECT_EQ(smartPtr.getWeakRefCount(), 1);

	Ash::RefCountPtr<A> smartPtr2 = Ash::RefCountPtr<A>(new A());
	smartPtr2 = nullptr; // should not have memory leak for m_counter
	EXPECT_EQ(A::s_instanceNum, 1);

	Ash::WeakRefPtr<A> weakPtr2 = std::move(weakPtr); // should not crash here
	EXPECT_EQ(weakPtr.getStrongRefCount(), 0);
	EXPECT_EQ(weakPtr.getWeakRefCount(), 0);
	EXPECT_EQ(weakPtr2.getStrongRefCount(), 1);
	EXPECT_EQ(weakPtr2.getWeakRefCount(), 1);

	Ash::RefCountPtr<A> nullA = Ash::RefCountPtr<B>(nullptr);// should not crash
	smartPtr = smartPtr.get();			// should not crash
	Ash::RefCountPtr<B> bPtr = Ash::RefCountPtr<B>(new B);
	bPtr->_b = Ash::RefCountPtr<B>(new B);	// should not crash
	bPtr = bPtr->_b;					// should not crash
	EXPECT_EQ(bPtr->getValue(), 233);	// should not crash
	bPtr.reset();
	{
		Ash::RefCountPtr<A> smartPtr2;
		smartPtr2 = smartPtr;
		EXPECT_EQ(smartPtr.getStrongRefCount(), 2);
		EXPECT_EQ(smartPtr.getWeakRefCount(), 1);

		Ash::WeakRefPtr<A> weakPtr2 = smartPtr;
		EXPECT_EQ(smartPtr.getStrongRefCount(), 2);
		EXPECT_EQ(smartPtr.getWeakRefCount(), 2);
		Ash::RefCountPtr<A> lockPtr = weakPtr2.lock(); //should not crash here
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

TEST(Ash, TaskRingBufferTest)
{
	const int THREAD_NUM = 5;
	constexpr size_t ITEM_NUM = 50000;
	struct Data
	{
		Data() :num(0), str("") {}
		Data(int num, const char* str) :num(num), str(str) {}
		std::string str;
		int num;
	};

	{
		TaskRingBuffer<Data> ringBuffer;

		std::vector<std::thread> push_threads;
		std::vector<std::thread> pop_threads;

		std::vector<Data> result_data{ ITEM_NUM };

		std::atomic_int push_index{ 0 };
		std::atomic_int pop_index{ 0 };

		auto start = std::chrono::system_clock::now();
		for (int i = 0; i < THREAD_NUM; ++i)
		{
			pop_threads.emplace_back([&pop_index, &ringBuffer, &result_data]()
			{
				while (true)
				{
					std::optional<Data> s = ringBuffer.popFront();
					if (!s.has_value())
						return;
					result_data[pop_index++] = std::move(s.value());
				}
			});
		}

		for (int i = 0; i < THREAD_NUM; ++i)
		{
			push_threads.emplace_back([&push_index, &ringBuffer, &ITEM_NUM]()
			{
				while (true)
				{
					int my_index = push_index++;
					if (my_index >= ITEM_NUM)
						return;

					char tmp[10];
					sprintf(tmp, "%d", my_index);
					ringBuffer.pushBack(std::move(Data(my_index,tmp)));
				}
			});
		}

		while (pop_index < ITEM_NUM)std::this_thread::yield();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		//DebugString("TaskRingBuffer: %f seconds", diff.count());
		ringBuffer.setExiting();
		for (auto& th : pop_threads)
		{
			th.join();
		}
		for (auto& th : push_threads)
		{
			th.join();
		}
		std::vector<bool> allItems((size_t)ITEM_NUM);
		for (int i = 0; i < ITEM_NUM; ++i)
		{
			int index = result_data[i].num;
			EXPECT_EQ(std::to_string(index), result_data[i].str);
			allItems[index] = true;
		}
		for (auto& b : allItems)
		{
			EXPECT_TRUE(b);
		}
	}
}


class RiNiMa : public Ash::RefCountObj
{
public:
	RiNiMa& then(std::function<void(RiNiMa& riNiMa)> worker...)
	{
		_data.emplace_back(std::move(worker));
	}
	void submit()
	{
		sPool.submit(Ash::RefCountPtr<RiNiMa>(this));
	}
	void onRun()
	{
		auto worker = std::move(_data.front());
		_data.pop_front();
		RiNiMa r;
		r._parent = this;
		worker(r);
        if(r.empty() && _data.empty())
        {
			_parent->onChildFinished();
        }
		else
		{
			sPool.submit(r);
		}
	}
	void onChildFinished()
	{
		--remaining;
		if(remaining == 0)
		{
			if (_data.empty())
			{
				_parent->onChildFinished();
			}
			else
			{
				ÏÂÒ»¸ö_data
			}
		}
	};
	std::list<std::function<void(RiNiMa&)>> _data;
	RiNiMa* _parent;
};

class TaskPool
{

public:
    void submit(Ash::RefCountPtr<RiNiMa> task)
    {
		_tasks.push_back(task);
    }
    void popTask()
    {
		auto task = _tasks.front();
		_tasks.pop_front();
		task->onRun();
    }

	std::list<Ash::RefCountPtr<RiNiMa>> _tasks;
};
static TaskPool sPool;

TEST(Ash, ThreadPoolTest)
{
	constexpr int threadNumber = 12;
	std::vector<int> numbers(20000000);
	for (auto& i : numbers)
	{
		i = (rand() & 0xfff) << 12 | (rand() & 0xfff);
	}


    auto B = [](RiNiMa& riNiMa)
    {
		riNiMa.then(F);
	};

    auto C = [](RiNiMa* )
    {
        
	};

	auto WTF = [&B, &C](RiNiMa& riNiMa)->RiNiMa&
	{
		riNiMa.then(B, C);
		riNiMa.then(D);
	};

	RiNiMa riNiMa;
	riNiMa.then(WTF);
	riNiMa.then([](RiNiMa&) {});

	WTF(riNiMa).submit();

	//MultiThread quick sort
	{
		std::function<void(Ash::TaskPtr, Ash::ThreadPool&, int*, int, int)> quickSort = [&quickSort](Ash::TaskPtr task, Ash::ThreadPool& threadPool, int* arr, int begin, int end)
		{
			if (end - begin < 32)
			{
				std::sort(arr + begin, arr + end);
				return;
			}
			int mid = (end - begin) / 2;
			if (arr[begin] > arr[mid])std::swap(arr[begin], arr[mid]);
			if (arr[mid] > arr[end - 1])std::swap(arr[mid], arr[end - 1]);
			if (arr[begin] > arr[mid])std::swap(arr[begin], arr[mid]);
			std::swap(arr[begin], arr[mid]);
			int guard = arr[begin];
			int left = begin + 1;
			for (int i = begin + 1; i < end; ++i)
			{
				if (arr[i] < guard)
				{
					std::swap(arr[left], arr[i]);
					++left;
				}
			}
			std::swap(arr[begin], arr[left - 1]);
			//quickSort(nullptr, arr, begin, left - 1);
			//quickSort(nullptr, arr, left, end);
			threadPool.dispatchTask(std::bind(quickSort, std::placeholders::_1, threadPool, arr, begin, left - 1), { task }, {});
			threadPool.dispatchTask(std::bind(quickSort, std::placeholders::_1, threadPool, arr, left, end), { task }, {});
		};

		std::vector<int> data = numbers;
        Ash::ThreadPool threadPool(threadNumber);
		auto start = std::chrono::system_clock::now();
		{
			std::atomic<bool> finished = false;
			Ash::TaskPtr sortTask = threadPool.dispatchTask(std::bind(quickSort, std::placeholders::_1, threadPool, data.data(), 0, (int)data.size()));
			std::mutex mu;
			std::unique_lock<std::mutex> lock(mu);
			std::condition_variable condi;
            Ash::TaskPtr endTask = threadPool.dispatchTask([&finished, &condi](Ash::TaskPtr)
			{
				finished = true; 
				condi.notify_all();
			}, {}, {sortTask});
			condi.wait(lock, [&finished]() {return finished.load(); });
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		DebugString("Sorted size=%ld random int array, MultiThread quick sort cost: %f seconds", data.size(), diff.count());
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

	// MultiThread merge sort
	if(1)
	{
		auto mergeArray = [](int* arr, int size)
		{
			int mid = size / 2;
			if (mid == size || mid == 0 || size == 0)
				return;
			std::shared_ptr<int[]> mergedArrPtr{ new int[size] };
			int* mergedArr = mergedArrPtr.get();

			int left = 0;
			int right = mid;
			int curr = 0;
			while (left < mid && right < size)
			{
				if (arr[left] < arr[right])
					mergedArr[curr++] = arr[left++];
				else
					mergedArr[curr++] = arr[right++];
			}
			while (left < mid)
				mergedArr[curr++] = arr[left++];
			while (right < size)
				mergedArr[curr++] = arr[right++];
			memcpy(arr, mergedArr, size * sizeof(int));
		};
		std::function<void(Ash::TaskPtr, Ash::ThreadPool&, int*,int)> mergeSort = [&mergeSort, &mergeArray](Ash::TaskPtr task, Ash::ThreadPool& threadPool, int* arr, int size)
		{
			if(size <= 32)
			{
				std::sort(arr, arr + size);	// small
			}
			else
			{
				int mid = size / 2;
				SomeFucking.Then(std::bind(mergeSort, std::placeholders::_1, threadPool, arr, mid), std::bind(mergeSort, std::placeholders::_1, threadPool, arr + mid, size - mid))
					.Then(std::bind(mergeArray, arr, size));

                Ash::TaskPtr leftTask = threadPool.dispatchTask(std::bind(mergeSort, std::placeholders::_1, threadPool, arr, mid), {task}, {});
                Ash::TaskPtr rightTask = threadPool.dispatchTask(std::bind(mergeSort, std::placeholders::_1, threadPool, arr + mid, size - mid), {task}, {});;
				Ash::TaskPtr mergeTask = threadPool.dispatchTask(std::bind(mergeArray, arr, size), {leftTask, rightTask}, {});
			}
		};
		//MultiThread merge sort
		{
			std::vector<int> data = numbers;
            Ash::ThreadPool threadPool(threadNumber);
			auto start = std::chrono::system_clock::now();
			{
				Ash::TaskPtr task = threadPool.dispatchTask(std::bind(mergeSort, std::placeholders::_1, threadPool, data.data(), (int)data.size()));
				
				std::atomic<bool> finished = false;
				std::mutex mu;
				std::unique_lock<std::mutex> lock(mu);
				std::condition_variable condi;
				threadPool.dispatchTask([&finished, &condi](Ash::TaskPtr)
					{
						finished = true;
						condi.notify_all();
					}, {}, {task});
				condi.wait(lock, [&finished]() {return finished.load(); });
			}
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			DebugString("Sorted size=%ld random int array, MultiThread merge sort cost: %f seconds", data.size(), diff.count());
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
	}

	////std::sort
	if(1)
	{
		auto start = std::chrono::system_clock::now();
		{
			std::sort(numbers.begin(), numbers.end(), std::less<int>());
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		DebugString("Sorted size=%ld random int array, std::sort cost: %f seconds", numbers.size(), diff.count());
	}
}