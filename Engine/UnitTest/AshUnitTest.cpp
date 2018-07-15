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
	std::vector<int> numbers(1000000);
	for (auto& i : numbers)
	{
		i = (rand() & 0xfff) << 12 | (rand() & 0xfff);
	}

	std::function<void(int*, int)> insertionSort = [](int* arr, int size)
	{
		for (int current = 1; current < size; ++current)
		{
			for (int i = current; i > 0; --i)
			{
				if (arr[i] < arr[i - 1])
					std::swap(arr[i], arr[i - 1]);
				else
					break;
			}
		}

	};

	std::function<void(int*,int, int) >  mergeArr = [](int* arr, int mid, int size)
	{
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

	//Test build task first
	if(1)
	{
		std::vector<int> data = numbers;
		ThreadPool threadPool(threadNumber);
		auto start = std::chrono::system_clock::now();
		{
			
			std::function <RefCountPtr<Task>(int* arr, int size)> buildTask = [&buildTask, &mergeArr, &insertionSort](int* arr, int size) -> RefCountPtr<Task>
			{
				RefCountPtr<Task> ret;
				if (size <= 32)
				{
					std::function<void(Task*)> taskFunInsertionSort = [arr, size, &insertionSort](Task*)
					{
						insertionSort(arr, size);
					};
					ret = RefCountPtr<Task>(new Task(taskFunInsertionSort));
				}
				else
				{
					int mid = size / 2;
					RefCountPtr<Task> sortTaskLeft = buildTask(arr, mid);
					RefCountPtr<Task> sortTaskRight = buildTask(arr + mid, size - mid);
					std::function<void(Task*)> mergeFun = [&mergeArr, mid, arr, size](Task*) {mergeArr(arr, mid, size); };
					RefCountPtr<Task> mergeTask{ new Task(mergeFun) };
					mergeTask->addDependence(sortTaskLeft);
					mergeTask->addDependence(sortTaskRight);
					ret = mergeTask;
				}
				return ret;
			};

			RefCountPtr<Task> mergeSortTask = buildTask(data.data(), (int)data.size());
			threadPool.addTask(mergeSortTask);
			mergeSortTask->waitForTaskDone();
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		DebugString("build task first cost: %f seconds", diff.count());
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

	//Test add task in task executing
	if(0)
	{
		std::vector<int> data = numbers;
		auto start = std::chrono::system_clock::now();
		{
			ThreadPool threadPool(threadNumber);
			std::function <void(Task*, int*, int)> createTaskWhenInTask = [&threadPool, &mergeArr, &createTaskWhenInTask, &insertionSort](Task*, int* arr, int size)
			{
				if (size <= 32)
				{
					insertionSort(arr, size);
				}
				else
				{
					int mid = size / 2;
					std::function<void(Task*)> leftFun = std::bind(createTaskWhenInTask, std::placeholders::_1, arr, mid);
					RefCountPtr<Task> sortTaskLeft(new Task(leftFun));
					std::function<void(Task*)> rightFun = std::bind(createTaskWhenInTask, std::placeholders::_1, arr + mid, size - mid);
					RefCountPtr<Task> sortTaskRight(new Task(rightFun));
					std::function<void(Task*)> mergeFun = [&mergeArr, mid, arr, size](Task*) {mergeArr(arr, mid, size); };
					RefCountPtr<Task> mergeTask{ new Task(mergeFun) };
					threadPool.addTask(sortTaskLeft);
					threadPool.addTask(sortTaskRight);
					//mergeTask->addDependence(sortTaskLeft);
					//mergeTask->addDependence(sortTaskRight);
					//threadPool.addTask(mergeTask);
				}
			};
			std::function <void(Task*)> firstTaskFun = std::bind(createTaskWhenInTask, std::placeholders::_1, data.data(), (int)data.size());
			RefCountPtr<Task> firstTask(new Task(firstTaskFun));
			threadPool.addTask(firstTask);
			threadPool.waitForAllTasksFinished();
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		DebugString("build task first cost: %f seconds", diff.count());
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

	//Test add task in different thread
	if(1)
	{
		const int insertionSortMaxSize = 32;
		const int addingTaskThreadNumber = 12;
		std::vector<int> data = numbers;
		ThreadPool threadPool(threadNumber);

		auto start = std::chrono::system_clock::now();
		{
			int perAddingTaskHandleTaskNum = (int)std::ceil(double(data.size()) / double(insertionSortMaxSize * addingTaskThreadNumber));
			for (int i = 0; i < addingTaskThreadNumber; ++i)
			{
				int begin = i * perAddingTaskHandleTaskNum * insertionSortMaxSize;
				if (begin >= (int)data.size())
					break;
				threadPool.addTask(RefCountPtr<Task>(new Task([insertionSortMaxSize, begin, perAddingTaskHandleTaskNum, &threadPool, &data, &insertionSort](Task*)
				{
					for (int j = 0; j < perAddingTaskHandleTaskNum; ++j)
					{
						int start = begin + j * insertionSortMaxSize;
						int end = start + insertionSortMaxSize;
						if (end > (int)data.size())
							end = (int)data.size();
						int size = end - start;
						std::function<void(Task*)> taskfun = [size, start, &data, &insertionSort](Task*)
						{
							insertionSort(data.data() + start, size);
						};
						threadPool.addTask(RefCountPtr<Task>(new Task(taskfun)));
					}
				})));
			}
			threadPool.waitForAllTasksFinished();

			int mergeSize = insertionSortMaxSize;
			while (true)
			{
				if (mergeSize >= (int)data.size())
					break;
				mergeSize *= 2;
				int perAddingTaskHandleTaskNum = (int)std::ceil(double(data.size()) / double(mergeSize * addingTaskThreadNumber));
				for (int i = 0; i < addingTaskThreadNumber; ++i)
				{
					int begin = i * perAddingTaskHandleTaskNum * mergeSize;
					if (begin >= (int)data.size())
						break;
					threadPool.addTask(RefCountPtr<Task>(new Task([mergeSize, begin, perAddingTaskHandleTaskNum, &threadPool, &data, &mergeArr](Task*)
					{
						for (int j = 0; j < perAddingTaskHandleTaskNum; ++j)
						{
							int start = begin + j * mergeSize;
							int end = start + mergeSize;
							if (end > (int)data.size())
								end = (int)data.size();
							int size = end - start;
							if (size <= 0)
								break;
							int mid = mergeSize / 2;
							if (mid >= size)
								break;
							std::function<void(Task*)> taskfun = [size, start, mid, &data, &mergeArr](Task*)
							{
								mergeArr(data.data() + start, mid, size);
							};
							threadPool.addTask(RefCountPtr<Task>(new Task(taskfun)));
						}
					})));
				}
				threadPool.waitForAllTasksFinished();
			}
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		DebugString("add task during running cost: %f seconds", diff.count());
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