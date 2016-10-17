#pragma once
#include "Ash/RefCountPointer/RefCountObj.h"

TINY_DEFINE_PTR(BaseComponent);
TINY_DEFINE_PTR(Object);
class Object;
class BaseComponent : public RefCountObj
{
public:
	virtual ~BaseComponent();
public:
	virtual ObjectID getObjectId() { return _id; }
	virtual void render() {}
	void setOwner(const RefCountPtr<Object> & owner);
	WeakRefPtr<Object> getOwner();
protected:
	BaseComponent();
	ObjectID _id;
	WeakRefPtr<Object> _owner;
};
