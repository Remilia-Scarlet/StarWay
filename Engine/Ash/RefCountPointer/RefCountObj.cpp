#include "precomp.h"
#include "RefCountObj.h"
#include "TinyEngine/Objects/Object.h"

ObjectID RefCountObj::s_nextId = INVALID_OBJECT_ID;

RefCountObj::RefCountObj()
	:_id(++s_nextId)
{
	_refInfo = new _RefInfo(this);
}

RefCountObj::~RefCountObj()
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
