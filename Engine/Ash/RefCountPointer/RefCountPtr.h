#pragma once
#include <type_traits>

class RefCountObj;
class _RefInfo;
template <class T>class WeakRefPtr;

template <class T>
class RefCountPtr
{
	friend class WeakRefPtr<T>;
public:
	typedef T ElementType;
public:
	RefCountPtr();
	explicit RefCountPtr(T* obj);
	RefCountPtr(const RefCountPtr<T>& other);
	template<class T2,
		class = typename std::enable_if<std::is_convertible<T2*, T*>::value, void>::type>
	RefCountPtr(const RefCountPtr<T2>& other);
	RefCountPtr(RefCountPtr<T>&& obj);
	virtual ~RefCountPtr();

	inline T* operator->() const;

	inline T& operator*() const;

	inline RefCountPtr<T>& operator=(T* obj);
	inline RefCountPtr<T>& operator=(const RefCountPtr<T>& other);
	template<class T2,
		class = typename std::enable_if<std::is_convertible<T2*, T*>::value, void>::type>
	inline RefCountPtr<T>& operator=(const RefCountPtr<T2>& other);
	inline RefCountPtr<T>& operator=(RefCountPtr<T>&& other);

	inline T* get()const;
	inline int getStrongRefCount()const;
	inline int getWeakRefCount()const;
	inline bool isValid()const;

	inline void swap(RefCountPtr<T>& other);

	inline void reset();
	inline void reset(T* ptr);
	inline void reset(const RefCountPtr<T>& other);
protected:
	T* _obj;
	_RefInfo* _refInfo;
};


template <class T>
class WeakRefPtr
{
public:
	typedef T ElementType;
public:
	WeakRefPtr();
	WeakRefPtr(T* ptr);
	template<class T2,
		class = typename std::enable_if<std::is_convertible<T2*, T*>::value, void>::type>
	WeakRefPtr(const RefCountPtr<T2>& strongPtr);
	WeakRefPtr(const RefCountPtr<T>& strongPtr);
	template<class T2,
		class = typename std::enable_if<std::is_convertible<T2*, T*>::value, void>::type>
	WeakRefPtr(const WeakRefPtr<T2>& other);
	WeakRefPtr(const WeakRefPtr<T>& other);
	WeakRefPtr(WeakRefPtr<T>&& other);
	virtual ~WeakRefPtr();

	inline WeakRefPtr<T>& operator=(T* ptr);
	template<class T2,
		class = typename std::enable_if<std::is_convertible<T2*, T*>::value, void>::type>
	inline WeakRefPtr<T>& operator=(const RefCountPtr<T2>& strongPtr);
	inline WeakRefPtr<T>& operator=(const RefCountPtr<T>& strongPtr);
	template<class T2,
		class = typename std::enable_if<std::is_convertible<T2*, T*>::value, void>::type>
	inline WeakRefPtr<T>& operator=(const WeakRefPtr<T2>& other);
	inline WeakRefPtr<T>& operator=(const WeakRefPtr<T>& other);
	inline WeakRefPtr<T>& operator=(WeakRefPtr<T>&& other);

	inline RefCountPtr<T> lock()const;
	inline int getStrongRefCount()const;
	inline int getWeakRefCount()const;
	inline bool isValid()const;

	inline void swap(WeakRefPtr<T>& other);

	inline void reset();
	inline void reset(T* ptr);
	inline void reset(const RefCountPtr<T>& other);
	inline void reset(const WeakRefPtr<T>& other);
protected:
	T* _obj;
	_RefInfo* _refInfo;
};


//////////////////////////////////////////////////////////////////////////
template <class T1, class T2>
bool operator<(const RefCountPtr<T1>& ptr, const T2* other)
{
	return ptr.get() < other;
}

template <class T1, class T2>
bool operator<(const T1* other, const RefCountPtr<T2>& ptr)
{
	return other < ptr.get();
}

template <class T1, class T2>
bool operator<(const RefCountPtr<T1>& ptr1, const RefCountPtr<T2>& ptr2)
{
	return ptr1.get() < ptr2.get();
}

template <class T>
bool operator<(const RefCountPtr<T>& ptr, std::nullptr_t)
{
	return ptr.get() < nullptr;
}

template <class T>
bool operator<(std::nullptr_t, const RefCountPtr<T>& ptr)
{
	return nullptr < ptr.get();
}

template<class T1, class T2>
bool operator==(const T1* obj, const RefCountPtr<T2>& ptr)
{
	return ptr.get() == obj;
}

template<class T1, class T2>
bool operator==(const RefCountPtr<T1>& ptr, const T2* obj)
{
	return ptr.get() == obj;
}

template<class T1, class T2>
bool operator==(const RefCountPtr<T1>& ptr1, const RefCountPtr<T2>& ptr2)
{
	return ptr1.get() == ptr2.get();
}

template <class T>
bool operator==(const RefCountPtr<T>& ptr, std::nullptr_t)
{
	return ptr.get() == nullptr;
}

template <class T>
bool operator==(std::nullptr_t, const RefCountPtr<T>& ptr)
{
	return nullptr == ptr.get();
}

template<class T1, class T2>
bool operator!=(const T1* obj, const RefCountPtr<T2>& ptr)
{
	return ptr.get() != obj;
}


template<class T1, class T2>
bool operator!=(const RefCountPtr<T1>& ptr, const T2* obj)
{
	return ptr.get() != obj;
}

template<class T1, class T2>
bool operator!=(const RefCountPtr<T1>& ptr1, const RefCountPtr<T2>& ptr2)
{
	return ptr1.get() != ptr2.get();
}

template <class T>
bool operator!=(const RefCountPtr<T>& ptr, std::nullptr_t)
{
	return ptr.get() != nullptr;
}

template <class T>
bool operator!=(std::nullptr_t, const RefCountPtr<T>& ptr)
{
	return nullptr != ptr.get();
}

//////////////////////////////////////////////////////////////////////////
template<class To,class From>
RefCountPtr<To> DynamicRefCountCast(const RefCountPtr<From>& ptr)
{
	To* toPtr = dynamic_cast<To*>(ptr.get());
	if (toPtr)
		return RefCountPtr<To>(toPtr);
	return RefCountPtr<To>();
}

//////////////////////////////////////////////////////////////////////////
#include "RefCountPtr.inl"