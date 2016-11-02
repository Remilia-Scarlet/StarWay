#pragma once
#include "ThirdParty/lua_5_3_3/lua.hpp"
#include "LuaVal.h"
class LuaFuns
{
public:
	static LuaFuns* instance();
	static int CreateScene(lua_State* L);
	static int CreateObject(lua_State* L);
	static int CreateTexture(lua_State* L);
	static int CreateTransform(lua_State* L);
	static int CreateCamera(lua_State* L);
	static int CreateScriptComponent(lua_State* L);
	static int SetLocation(lua_State* L);
	static int SetRotation(lua_State* L);
	static int FaceToPoint(lua_State* L);
	static int AddObject(lua_State* L);

	static int GenerateCubeMesh(lua_State* L);
	static int GenerateSphereMesh(lua_State* L);
	static int AddComponent(lua_State* L);
	static int StartScene(lua_State* L);

	bool registerFuncsToLua();
	static bool pushVal(lua_State * L, const LuaVal& val);
	static LuaVal getVal(lua_State * L, int index);
};

