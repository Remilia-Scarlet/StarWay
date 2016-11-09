#pragma once
#include "TinyEngine\Engine\EngineDefs.h"
#include <unordered_map>
#include <type_traits>
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include <type_traits>
#include <bitset>
#include "TinyEngine/ScriptManager/LuaManager.h"

TINY_DEFINE_PTR(Object);
TINY_DEFINE_PTR(BaseComponent);
class BaseComponent;

enum class ObjectFlag
{
	IS_CAMERA,   //check if this obj is a camera
	IS_LIGHT
};

class Object : public RefCountObj
{
public:
	static bool createLuaPrototype();
public:
	static ObjectPtr create();
	LUA_CREATE_FUN_P0(Object);

	virtual void addChild(ObjectPtr child);
	virtual void removeChild(ObjectPtr child);

	virtual ObjectWeakPtr getParent() { return _parent; }

	virtual void addComponent(const BaseComponentPtr& component);
	LUA_MEMBER_FUN_P1R0_DECL(Object, addComponent, const BaseComponentPtr&);

//	static int L_addComponent(lua_State* L);
	template<class T> RefCountPtr<T> getComponent();
	BaseComponentPtr getComponent(ObjectID componentId);
	template<class T> std::vector<RefCountPtr<T> > getComponents();
	BaseComponentPtr getComponent(const std::string& name);
	static int L_getComponent(lua_State* L);

	virtual void update(float dt);
	virtual void render();

	virtual void setFlag(ObjectFlag flagType, bool val);
	virtual bool getFlag(ObjectFlag flagType);
	virtual int getFlags();

	virtual void setEnable(bool enable);
	LUA_MEMBER_FUN_P1R0_DECL(Object, setEnable, bool);

	virtual bool getEnable();
	LUA_MEMBER_FUN_P0R1_DECL(Object, getEnable);
protected:
	void ensureChildMap();
	void ensureComponentMap();
	void Object::setParent(ObjectPtr parent);
	virtual bool init();

	bool _enable;
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

