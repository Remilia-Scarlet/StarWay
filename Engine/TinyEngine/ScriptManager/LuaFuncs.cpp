#include "TinyEngine/precomp.h"
#include "LuaFuncs.h"
#include "LuaManager.h"
#include <functional>
#include "TinyEngine/Scene/Scene.h"


#define REGIST_FUN(_NAME_) do{instance()->_funcs[#_NAME_] = LuaFuns::_NAME_;}while(0)

LuaFuns* LuaFuns::instance()
{
	static LuaFuns instance;
	return &instance;
}

int LuaFuns::CreateScene(lua_State* L)
{
	REGIST_FUN(CreateScene);
	ScenePtr scene = Scene::create();
	LuaManager::instance()->pushVal(scene);
	return 1;
}

int LuaFuns::CreateObject(lua_State* L)
{
	REGIST_FUN(CreateObject);
	ObjectPtr obj = Object::create();
	LuaManager::instance()->pushVal(obj);
	return 1;
}

bool LuaFuns::registerFuncsToLua()
{
	lua_State* L = LuaManager::instance()->_LuaState;
	for (auto& pair : _funcs)
	{
		lua_register(L, pair.first, pair.second);
	}
	return true;
}
