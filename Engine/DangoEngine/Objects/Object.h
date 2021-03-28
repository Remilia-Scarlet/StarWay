#pragma once
#include "DangoEngine/Engine/EngineDefs.h"
#include <unordered_map>
#include <type_traits>
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include <type_traits>
#include <bitset>

#include "Ash/MultiThread/FunctorSeq.h"


ASH_DEFINE_PTR(Object);
ASH_DEFINE_PTR(BaseComponent);
class BaseComponent;

enum class ObjectFlag
{
	IS_CAMERA,   //check if this obj is a camera
	IS_LIGHT,
	IS_DIRECTIONAL_LIGHT,
	IS_POINT_LIGHT,
	IS_SPOT_LIGHT
};

class Object : public Ash::RefCountObj
{
public:
	static ObjectPtr create(const std::string& objName);
	static ObjectPtr create();

	virtual void addChild(ObjectPtr child);
	virtual void removeChild(ObjectPtr child);

	virtual ObjectWeakPtr getParent() { return _parent; }

	virtual void addComponent(const BaseComponentPtr& component);

	template<class T>
    Ash::RefCountPtr<T> getComponent();
	BaseComponentPtr getComponent(Ash::ObjectID componentId);
	template<class T> std::vector<Ash::RefCountPtr<T> > getComponents();
	BaseComponentPtr getComponent(const std::string& name);

	virtual void update(Ash::FunctorSeq& task, float dt);
	virtual void render();

	virtual void setFlag(ObjectFlag flagType, bool val);
	virtual bool getFlag(ObjectFlag flagType);
	virtual int getFlags();

	virtual void setEnable(bool enable);
	virtual bool getEnable();
protected:
	void ensureChildMap();
	void ensureComponentMap();
	void Object::setParent(ObjectPtr parent);
	virtual bool init();
	virtual bool init(const std::string& objName);

	bool _enable;
	std::string _name;
	ObjectWeakPtr _parent;
	std::unordered_map<Ash::ObjectID, ObjectPtr>* _children;
	std::unordered_map<Ash::ObjectID, BaseComponentPtr>* _components;
	std::bitset<32> _flag;
public:
	Object();
	virtual ~Object();
};

template<class T>
Ash::RefCountPtr<T> Object::getComponent()
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
std::vector<Ash::RefCountPtr<T> > Object::getComponents()
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

