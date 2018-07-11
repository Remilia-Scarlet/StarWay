#pragma once
#include "Ash/CommonFunc.h"
#include "Ash/TinyAssert.h"
#include <atomic>

template <class T> class RefCountPtr;
class RefCountObj;

class _RefInfo
{
public:
	_RefInfo(RefCountObj* obj) :_refCount(0), _weakCount(0), _obj(obj) {}
	std::atomic<int> _refCount;
	std::atomic<int> _weakCount;
	RefCountObj* _obj;

	inline void addStrongRef();
	inline void releaseStrongRef();
	inline void addWeakRef();
	inline void releaseWeakRef();

	inline int getStrongRefCount();
	inline int getWeakRefCount();

	inline bool isValid();
};


class RefCountObj
{
	template <class T>friend class RefCountPtr;
	template <class T>friend class WeakRefPtr;
	friend class _RefInfo;
public:
	int getStrongRefCount()const { return _refInfo->getStrongRefCount(); }
	int getWeakRefCount()const { return _refInfo->getWeakRefCount(); }
	virtual ObjectID getObjectId()  const { return _id; }
protected:
	explicit RefCountObj();
	virtual ~RefCountObj();
private:
	_RefInfo* _refInfo;
	ObjectID _id;
	static ObjectID s_nextId;
};



//////////////////////////////////////////////////////////////////////////

void _RefInfo::addStrongRef()
{
	++_refCount;
}

void _RefInfo::releaseStrongRef()
{
	TinyAssert(_refCount > 0);
	int currentVal = --_refCount;
	if (currentVal == 0)
	{
		addWeakRef();// for not delete _RefInfo
		_obj->_refInfo = nullptr;
		delete _obj;
		_obj = nullptr;
		releaseWeakRef();
	}
}

void _RefInfo::addWeakRef()
{
	++_weakCount;
}

void _RefInfo::releaseWeakRef()
{
	TinyAssert(_weakCount > 0);
	int currentWeak = --_weakCount;
	if (_refCount == 0 && currentWeak == 0)
		delete this;
}

int _RefInfo::getStrongRefCount()
{
	return _refCount;
}

int _RefInfo::getWeakRefCount()
{
	return _weakCount;
}

bool _RefInfo::isValid()
{
	return _refCount > 0;
}
