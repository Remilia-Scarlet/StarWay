#include "RefCountObj.h"

ObjectID Ash::RefCountObj::s_nextId = INVALID_OBJECT_ID;

Ash::RefCountObj::RefCountObj()
	:_id(++s_nextId)
{
	_refInfo = new _RefInfo(this);
}

Ash::RefCountObj::~RefCountObj()
{
	if (_refInfo)
	{
		if (_refInfo->_refCount || _refInfo->_weakCount)
		{
			TinyAssert(false, "Don't delete RefCountObj when it is managed by RefCountPtr. Your code will crash soon");
		}
		delete _refInfo;
	}
}

void Ash::RefCountObj::addRef()
{
	if(_refInfo)
	{
		_refInfo->addStrongRef();
	}
}

void Ash::RefCountObj::releaseRef()
{
	if(_refInfo)
	{
		_refInfo->releaseStrongRef();
	}
}
