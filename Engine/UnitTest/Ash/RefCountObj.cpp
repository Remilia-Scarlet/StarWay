#include "gtest/gtest.h"

#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"


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
		Ash::RefCountPtr<A> smartPtr3;
		smartPtr3 = smartPtr;
		EXPECT_EQ(smartPtr.getStrongRefCount(), 2);
		EXPECT_EQ(smartPtr.getWeakRefCount(), 1);

		Ash::WeakRefPtr<A> weakPtr3 = smartPtr;
		EXPECT_EQ(smartPtr.getStrongRefCount(), 2);
		EXPECT_EQ(smartPtr.getWeakRefCount(), 2);
		Ash::RefCountPtr<A> lockPtr = weakPtr3.lock(); //should not crash here
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
