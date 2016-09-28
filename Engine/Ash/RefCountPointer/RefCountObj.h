#pragma once

template <class T> class RefCountPtr;
class RefCountObj;

class _RefInfo
{
public:
	_RefInfo(RefCountObj* obj) :_refCount(0), _weakCount(0), _obj(obj) {}
	int _refCount;
	int _weakCount;
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
protected:
	explicit RefCountObj();
	virtual ~RefCountObj();
private:
	_RefInfo* _refInfo;
};



//////////////////////////////////////////////////////////////////////////

void _RefInfo::addStrongRef()
{
	++_refCount;
}

void _RefInfo::releaseStrongRef()
{
	if (_refCount > 0)
	{
		--_refCount;
		if (_refCount == 0)
		{
			_obj->_refInfo = nullptr;
			delete _obj;
			_obj = nullptr;
		}
		if (_refCount == 0 && _weakCount == 0)
		{
			delete this;
		}
	}
}

void _RefInfo::addWeakRef()
{
	++_weakCount;
}

void _RefInfo::releaseWeakRef()
{
	if (_weakCount > 0)
	{
		--_weakCount;
		if (_refCount == 0 && _weakCount == 0)
			delete this;
	}
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
