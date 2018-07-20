#pragma once
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/MultiThread/Task.h"

TINY_DEFINE_PTR(BaseComponent);
TINY_DEFINE_PTR(Object);
class Object;
class BaseComponent : public RefCountObj
{
public:
	virtual ~BaseComponent();
public:
	virtual const std::string& getName() const { return _name; }
	virtual void update(Task* task, float dt) {}
	virtual void render() {}
	virtual void setOwner(const RefCountPtr<Object> & owner);
	virtual WeakRefPtr<Object> getOwner() const;
protected:
	BaseComponent(const char* name);
	WeakRefPtr<Object> _owner;
	std::string _name;
};
