#pragma once
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/MultiThread/Task.h"

ASH_DEFINE_PTR(BaseComponent);
ASH_DEFINE_PTR(Object);
class Object;
class BaseComponent : public Ash::RefCountObj
{
public:
	virtual ~BaseComponent();
public:
	virtual const std::string& getName() const { return _name; }
	virtual void update(Task* task, float dt) {}
	virtual void render() {}
	virtual void setOwner(const Ash::RefCountPtr<Object> & owner);
	virtual Ash::WeakRefPtr<Object> getOwner() const;
protected:
	BaseComponent(const char* name);
    Ash::WeakRefPtr<Object> _owner;
	std::string _name;
};
