#include "RefCountObj.h"
#include <assert.h>

RefCountObj::RefCountObj()
{
	_refInfo = new _RefInfo(this);
}

RefCountObj::~RefCountObj()
{
	if (_refInfo)
	{
		if (_refInfo->_refCount || _refInfo->_weakCount)
		{
		
			//don't delete RefCountObj when it is managed by RefCountPtr. Your code will crash soon
			assert(false);
		}
		delete _refInfo;
	}
}
