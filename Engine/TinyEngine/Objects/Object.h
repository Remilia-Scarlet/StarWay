#pragma once
#include "TinyEngine\Engine\EngineDefs.h"
#include <unordered_map>
#include <type_traits>
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include <type_traits>
#include <bitset>

TINY_DEFINE_PTR(Object);
TINY_DEFINE_PTR(BaseComponent);
class BaseComponent;

enum class ObjectFlag
{
	IS_CAMERA = 0   //check if this obj is a camera
};

class Object : public RefCountObj
{
public:
	static ObjectPtr create();
	virtual void addChild(ObjectPtr child);
	virtual void removeChild(ObjectPtr child);

	virtual ObjectWeakPtr getParent() { return _parent; }

	virtual void addComponent(const BaseComponentPtr& component);
	template<class T> RefCountPtr<T> getComponent();
	BaseComponentPtr getComponent(ObjectID componentId);
	template<class T> std::vector<RefCountPtr<T> > getComponents();

	virtual void update(double dt) {}
	virtual void render();

	virtual ObjectID getObjectId() { return _id; }

	virtual void setFlag(ObjectFlag flagType, bool val);
	virtual bool getFlag(ObjectFlag flagType);
protected:
	void ensureChildMap();
	void ensureComponentMap();
	void Object::setParent(ObjectPtr parent);
	virtual bool init();
	ObjectID _id;
	ObjectWeakPtr _parent;
	std::unordered_map<ObjectID, ObjectPtr>* _children;
	std::unordered_map<ObjectID, BaseComponentPtr>* _components;
	std::bitset<32> _flag;
public:
	Object();
	virtual ~Object();
};

template<class T>
RefCountPtr<T> Object::getComponent()
{
	static_assert(std::is_convertible<T*,const BaseComponent*>::value,"This type must be BaseComponent!");
	if (_components)
	{
		RefCountPtr<T> ptr;
		for (auto it = _components->begin(); it != _components->end(); ++it)
		{
			ptr = DynamicRefCountCast<T>(it->second);
			if (ptr.isValid())
				return ptr;
		}
	}
	return RefCountPtr<T>();
}

template<class T>
std::vector<RefCountPtr<T> > Object::getComponents()
{
	static_assert(std::is_convertible<T*, const BaseComponent*>::value, "This type must be BaseComponent!");
	std::vector<RefCountPtr<T> > ret;
	if (_components)
	{
		RefCountPtr<T> ptr;
		for (auto it = _components->begin(); it != _components->end(); ++it)
		{
			ptr = DynamicRefCountCast<T>(it->second);
			if (ptr.isValid())
				ret.push_back(ptr);
		}
	}
	return ret;
}

