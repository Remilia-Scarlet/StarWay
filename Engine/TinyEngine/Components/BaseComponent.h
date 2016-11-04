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
	virtual const std::string& getName() const { return _name; }
	virtual ObjectID getObjectId()  const { return _id; }
	virtual void update(float dt) {}
	virtual void render() {}
	virtual void setOwner(const RefCountPtr<Object> & owner);
	virtual WeakRefPtr<Object> getOwner() const;
protected:
	BaseComponent(const char* luaPrototypeName);
	ObjectID _id;
	WeakRefPtr<Object> _owner;
	std::string _name;
};
