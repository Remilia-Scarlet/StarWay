#include "RefCountPtr.h"
//////////////////////////////////////////////////////////////////////////
template <class T>
T* RefCountPtr<T>::operator->() const
{
	return _obj;
}

template <class T>
T& RefCountPtr<T>::operator*() const
{
	return *_obj;
}

template <class T>
RefCountPtr<T>& RefCountPtr<T>::operator=(T* obj)
{
	reset(obj);
	return *this;
}

template <class T>
template<class T2, class>
RefCountPtr<T>& RefCountPtr<T>::operator=(const RefCountPtr<T2>& other)
{
	reset(other.get());
	return *this;
}

template <class T>
RefCountPtr<T>& RefCountPtr<T>::operator=(const RefCountPtr<T>& other)
{
	reset(other);
	return *this;
}

template <class T>
RefCountPtr<T>& RefCountPtr<T>::operator=(RefCountPtr<T>&& other)
{
	reset();
	swap(other);
	return *this;
}

template <class T>
bool RefCountPtr<T>::isValid() const
{
	return _refInfo && _refInfo->isValid();
}

template <class T>
int RefCountPtr<T>::getStrongRefCount() const
{
	return (_refInfo ? _refInfo->getStrongRefCount() : 0);
}

template <class T>
int RefCountPtr<T>::getWeakRefCount() const
{
	return (_refInfo ? _refInfo->getWeakRefCount() : 0);
}

template <class T>
T* RefCountPtr<T>::get() const
{
	return _obj;
}

template <class T>
void RefCountPtr<T>::reset()
{
	reset(nullptr);
}

template <class T>
void RefCountPtr<T>::reset(T* ptr)
{
	if (_obj != ptr)
	{
		if (ptr)
			ptr->_refInfo->addStrongRef();
		if (_refInfo)
			_refInfo->releaseStrongRef();
		_obj = ptr;
		_refInfo = (ptr ? ptr->_refInfo : nullptr);
	}
}

template <class T>
void RefCountPtr<T>::reset(const RefCountPtr& other)
{
	reset(other._obj);
}

template <class T>
void RefCountPtr<T>::swap(RefCountPtr& other)
{
	T* obj = _obj;
	_obj = other._obj;
	other._obj = obj;

	_RefInfo* info = _refInfo;
	_refInfo = other._refInfo;
	other._refInfo = info;
}

template <class T>
RefCountPtr<T>::RefCountPtr()
	: _obj(nullptr), _refInfo(nullptr)
{
}

template<class T>
inline RefCountPtr<T>::RefCountPtr(std::nullptr_t)
	: _obj(nullptr), _refInfo(nullptr)
{
}

template <class T>
RefCountPtr<T>::RefCountPtr(T* obj)
	: _obj(nullptr), _refInfo(nullptr)
{
	reset(obj);
}

template <class T>
RefCountPtr<T>::RefCountPtr(const RefCountPtr<T>& other)
	:_obj(nullptr), _refInfo(nullptr)
{
	reset(other.get());
}

template< class T >
template <class T2, class>
RefCountPtr<T>::RefCountPtr(const RefCountPtr<T2>& other)
	: _obj(nullptr), _refInfo(nullptr)
{
	reset(other.get());
}

template <class T>
RefCountPtr<T>::RefCountPtr(RefCountPtr<T>&& other)
	: _obj(nullptr), _refInfo(nullptr)
{
	swap(other);
}

template <class T>
RefCountPtr<T>::~RefCountPtr()
{
	if (_refInfo)
		_refInfo->releaseStrongRef();
}


//////////////////////////////////////////////////////////////////////////
//		WeakRefPtr		//
//////////////////////////////////////////////////////////////////////////
template <class T>
WeakRefPtr<T>::WeakRefPtr()
	:_obj(nullptr)
	,_refInfo(nullptr)
{

}

template <class T>
WeakRefPtr<T>::WeakRefPtr(T* ptr)
	:_obj(nullptr)
	, _refInfo(nullptr)
{
	reset(ptr);
}

template <class T>
template <class T2 , class>
WeakRefPtr<T>::WeakRefPtr(const RefCountPtr<T2>& strongPtr)
	:_obj(nullptr)
	, _refInfo(nullptr)
{
	reset(strongPtr.get());
}

template <class T>
WeakRefPtr<T>::WeakRefPtr(const RefCountPtr<T>& strongPtr)
	:_obj(nullptr)
	, _refInfo(nullptr)
{
	reset(strongPtr);
}

template <class T>
template <class T2, class>
WeakRefPtr<T>::WeakRefPtr(const WeakRefPtr<T2>& other)
	:_obj(nullptr)
	, _refInfo(nullptr)
{
	reset(other.lock().get());
}

template <class T>
WeakRefPtr<T>::WeakRefPtr(const WeakRefPtr<T>& other)
	:_obj(nullptr)
	, _refInfo(nullptr)
{
	reset(other);
}

template <class T>
WeakRefPtr<T>::WeakRefPtr(WeakRefPtr<T>&& other)
	:_obj(nullptr)
	, _refInfo(nullptr)
{
	swap(other);
}

template <class T>
WeakRefPtr<T>::~WeakRefPtr()
{
	if (_refInfo)
		_refInfo->releaseWeakRef();
}

template <class T>
WeakRefPtr<T>& WeakRefPtr<T>::operator=(T* ptr)
{
	reset(ptr);
	return *this;
}

template <class T>
template <class T2,class>
WeakRefPtr<T>& WeakRefPtr<T>::operator=(const RefCountPtr<T2>& strongPtr )
{
	reset(strongPtr.get());
	return *this;
}

template <class T>
WeakRefPtr<T>& WeakRefPtr<T>::operator=(const RefCountPtr<T>& strongPtr)
{
	reset(strongPtr);
	return *this;
}

template <class T>
template <class T2, class>
WeakRefPtr<T>& WeakRefPtr<T>::operator=(const WeakRefPtr<T2>& other)
{
	reset(other.lock().get());
	return *this;
}

template <class T>
WeakRefPtr<T>& WeakRefPtr<T>::operator=(const WeakRefPtr<T>& other)
{
	reset(other);
	return *this;
}

template <class T>
WeakRefPtr<T>& WeakRefPtr<T>::operator=(WeakRefPtr<T>&& other)
{
	reset();
	swap(other);
	return *this;
}

template <class T>
RefCountPtr<T> WeakRefPtr<T>::lock() const
{
	if (isValid())
		return RefCountPtr<T>(_obj);
	return RefCountPtr<T>();
}

template <class T>
int WeakRefPtr<T>::getStrongRefCount() const
{
	return _refInfo ? _refInfo->getStrongRefCount() : 0;
}

template <class T>
int WeakRefPtr<T>::getWeakRefCount() const
{
	return _refInfo ? _refInfo->getWeakRefCount() : 0;
}

template <class T>
bool WeakRefPtr<T>::isValid() const
{
	return _refInfo && _refInfo->isValid();
}

template <class T>
void WeakRefPtr<T>::swap(WeakRefPtr<T>& other)
{
	T* obj = _obj;
	_obj = other._obj;
	other._obj = obj;

	_RefInfo* info = _refInfo;
	_refInfo = other._refInfo;
	other._refInfo = info;
}

template <class T>
void WeakRefPtr<T>::reset()
{
	reset(nullptr);
}

template <class T>
void WeakRefPtr<T>::reset(T* ptr)
{
	if(ptr && ptr->_refInfo->isValid())
		ptr->_refInfo->addWeakRef();
	if (_refInfo != nullptr)
		_refInfo->releaseWeakRef();
	if (ptr && ptr->_refInfo->isValid())
	{
		_obj = ptr;
		_refInfo = ptr->_refInfo;
	}
	else
	{
		_obj = nullptr;
		_refInfo = nullptr;
	}
}

template <class T>
void WeakRefPtr<T>::reset(const RefCountPtr<T>& other)
{
	reset(other._obj);
}

template <class T>
void WeakRefPtr<T>::reset(const WeakRefPtr<T>& other)
{
	reset(other.isValid() ? other._obj : nullptr);
}