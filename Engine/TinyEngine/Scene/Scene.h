#pragma once
#include "TinyEngine/Objects/Object.h"
#include <unordered_map>
#include <functional>
#include "Ash/RefCountPointer/RefCountObj.h"
#include "TinyEngine/ScriptManager/LuaManager.h"
TINY_DEFINE_PTR(Scene);

class Scene : public RefCountObj
{
public:
	static bool createLuaPrototype();
public:
	Scene();
	virtual ~Scene();
public:
	static ScenePtr create();
	LUA_CREATE_FUN_P0(Scene);

	virtual void addObject(ObjectPtr obj);
	static int L_addObject(lua_State* L);
//@@TODO:	virtual void removeObject(...);

	virtual void update(float dt);
	virtual void render();


	virtual bool start() { return true; }
protected:
	virtual bool init();
	std::unordered_map<ObjectID, ObjectWeakPtr> _cameras;
	std::unordered_map<ObjectID, ObjectWeakPtr> _lights;
	std::unordered_map<ObjectID,ObjectPtr> _objects;

};

