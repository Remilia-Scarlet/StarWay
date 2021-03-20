#include "gtest/gtest.h"

#include "Ash/Container/RingBuffer.h"


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
