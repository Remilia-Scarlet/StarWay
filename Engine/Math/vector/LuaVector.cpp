#define _CRT_SECURE_NO_WARNINGS
#include <unordered_map>
#include "LuaVector.h"

bool LuaVector::createLuaPrototype()
{
	lua_State* L = LuaManager::instance()->getLuaMachine();
	int oldStack = lua_gettop(L);
	lua_newtable(L);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, &LuaVector::L_index);
	lua_rawset(L, -3);

	LUA_PROTOTYPE_REGIST_FUN(create);

	lua_pushstring(L, "__add");
	lua_pushcfunction(L, &LuaVector::L_add);
	lua_rawset(L, -3);

	

	LUA_PROTOTYPE_END(LuaVector);
	return true;
}

int LuaVector::L_index(lua_State* L)
{
	int oldStack = lua_gettop(L);

	if (lua_type(L, 1) != LUA_TUSERDATA)
	{
		lua_pushnil(L);
		return 1;
	}

	if (lua_type(L, 2) != LUA_TSTRING)
	{
		lua_pushnil(L);
		return 1;
	}

	const char* keyStr = lua_tostring(L, 2);
	int keyIndex = -1;
	if (strcmp(keyStr, "x") == 0)
		keyIndex = 0;
	else if (strcmp(keyStr, "y") == 0)
		keyIndex = 1;
	else if (strcmp(keyStr, "z") == 0)
		keyIndex = 2;
	else if (strcmp(keyStr, "w") == 0)
		keyIndex = 3;
	else
	{
		lua_pushnil(L);
		return 1;
	}

	size_t size = lua_rawlen(L, 1);
	void* data = lua_touserdata(L, 1);
	constexpr size_t vecSize[4] = { sizeof(Vector2),sizeof(Vector2) ,sizeof(Vector3) ,sizeof(Vector4) };
	if (size < vecSize[keyIndex])
	{
		lua_pushnil(L);
		return 1;
	}
	lua_pushnumber(L, *((float*)data + keyIndex));
	TinyAssert(oldStack + 1 == lua_gettop(L));
	return 1;
}

int LuaVector::L_create(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop < 2)
		return LUA_PARAM_ERROR(LuaVector::L_create);

	TinyAssert(lua_type(L, 2) == LUA_TNUMBER);
	lua_Integer size = lua_tointeger(L, 2);
	if (size <= 1)
		return LUA_PARAM_ERROR(LuaVector::L_create);

	float val[4] = { 0,0,0,0 };
	for (int i = 3; i <= oldTop && i - 3 < 4; ++i)
	{
		val[i - 3] = (float)lua_tonumber(L, i);
	}

	switch (size)
	{
		case 2:
		{
			void* data = allocAndPushUserData(sizeof(Vector2), 1);
			new (data) Vector2(val);
			break;
		}
		case 3:
		{
			void* data = allocAndPushUserData(sizeof(Vector3), 1);
			new (data) Vector3(val);
			break;
		}
		case 4:
		{
			void* data = allocAndPushUserData(sizeof(Vector4), 1);
			new (data) Vector4(val);
			break;
		}
		default:
			TinyAssert(false, "unreachable code");
	}
	
	lua_pushnil(L);
	lua_copy(L, 1, -1);
	lua_setmetatable(L, -2);
	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

inline int LuaVector::L_add(lua_State * L)
{
	int oldTop = lua_gettop(L);
	if (oldTop != 2)
		return LUA_PARAM_ERROR(VectorStorage::L_add);
	if(lua_type(L, 1) != LUA_TUSERDATA || lua_type(L, 2) != LUA_TUSERDATA)
		return LUA_PARAM_ERROR(VectorStorage::L_add);
	size_t size = lua_rawlen(L, 1);
	if(size != lua_rawlen(L, 2))
		return LUA_PARAM_ERROR(VectorStorage::L_add);
	switch (size)
	{
		case sizeof(Vector2) :
		{
			*(Vector2*)allocAndPushUserData(sizeof(Vector2)) = *(Vector2*)lua_touserdata(L, 1) + *(Vector2*)lua_touserdata(L, 2);
			break;
		}
		case sizeof(Vector3) :
		{
			*(Vector3*)allocAndPushUserData(sizeof(Vector3)) = *(Vector3*)lua_touserdata(L, 1) + *(Vector3*)lua_touserdata(L, 2);
			break;
		}
		case sizeof(Vector4) :
		{
			*(Vector4*)allocAndPushUserData(sizeof(Vector4)) = *(Vector4*)lua_touserdata(L, 1) + *(Vector4*)lua_touserdata(L, 2);
			break;
		}
		default:
			return LUA_PARAM_ERROR(VectorStorage::L_add);
	}
	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

void* LuaVector::allocAndPushUserData(int size,int metaIndex)
{
	lua_State* L = LuaManager::instance()->getLuaMachine();
	int absIndex = lua_absindex(L, metaIndex);
	void* data = lua_newuserdata(L, size);
	lua_pushnil(L);
	lua_copy(L, absIndex, -1);
	lua_setmetatable(L, -2);
	return data;
}

void* LuaVector::allocAndPushUserData(int size)
{
	lua_State* L = LuaManager::instance()->getLuaMachine();
	int type = lua_getglobal(L, TO_STRING(LuaVector));
	TinyAssert(type == LUA_TTABLE);
	void* data = allocAndPushUserData(size, -1);
	lua_replace(L, -2);
	return data;
}
