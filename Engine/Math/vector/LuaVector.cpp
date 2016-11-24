#define _CRT_SECURE_NO_WARNINGS
#include <unordered_map>
#include "LuaVector.h"

bool LuaVector::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);

	lua_pushstring(L, "__add");
	lua_pushcfunction(L, &LuaVector::L_add);
	lua_rawset(L, -3);

	

	LUA_PROTOTYPE_END(LuaVector);
	return true;
}

int LuaVector::L_create(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop < 2)
		return LUA_PARAM_ERROR(LuaVector::L_create);

	TinyAssert(lua_type(L, 2) == LUA_TNUMBER);
	lua_Integer size = lua_tointeger(L, 2);
	if (size <= 1 || size > 4)
		return LUA_PARAM_ERROR(LuaVector::L_create);

	switch (size)
	{
		case 2:
		{
			void* data = allocAndPushUserData(sizeof(Vector2));
			new (data) Vector2();
			break;
		}
		case 3:
		{
			void* data = allocAndPushUserData(sizeof(Vector3));
			new (data) Vector3();
			break;
		}
		case 4:
		{
			void* data = allocAndPushUserData(sizeof(Vector4));
			new (data) Vector4();
			break;
		}
		default:
			TinyAssert(false, "unreachable code");
	}
	
	lua_pushnil(L);
	lua_copy(L, -4, -1);
	lua_setmetatable(L, -2);
	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

int LuaVector::L_x(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop < 1)
		return LUA_PARAM_ERROR(LuaVector::L_x);

	if(lua_type(L, 1) != LUA_TUSERDATA)
		return LUA_PARAM_ERROR(LuaVector::L_x);


	void* data = lua_touserdata(L, 1);
	size_t size = lua_rawlen(L, 1);
	switch (size)
	{
	case sizeof(Vector2) :
	case sizeof(Vector3) :
	case sizeof(Vector4) :
		lua_pushnumber(L, *(float*)data);
		break;
	default:
		return LUA_PARAM_ERROR(LuaVector::L_x);
	}

	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

int LuaVector::L_y(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop < 1)
		return LUA_PARAM_ERROR(LuaVector::L_y);

	if (lua_type(L, 1) != LUA_TUSERDATA)
		return LUA_PARAM_ERROR(LuaVector::L_y);


	void* data = lua_touserdata(L, 1);
	size_t size = lua_rawlen(L, 1);
	switch (size)
	{
	case sizeof(Vector2) :
	case sizeof(Vector3) :
	case sizeof(Vector4) :
		lua_pushnumber(L, *((float*)data + 1));
		break;
	default:
		return LUA_PARAM_ERROR(LuaVector::L_y);
	}

	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

int LuaVector::L_z(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop < 1)
		return LUA_PARAM_ERROR(LuaVector::L_z);

	if (lua_type(L, 1) != LUA_TUSERDATA)
		return LUA_PARAM_ERROR(LuaVector::L_z);


	void* data = lua_touserdata(L, 1);
	size_t size = lua_rawlen(L, 1);
	switch (size)
	{
	case sizeof(Vector2) :
		return luaL_error(L, "Vector2 doesn't have z field");
	case sizeof(Vector3) :
	case sizeof(Vector4) :
		lua_pushnumber(L, *((float*)data + 1));
		break;
	default:
		return LUA_PARAM_ERROR(LuaVector::L_y);
	}

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

void* LuaVector::allocAndPushUserData(int size)
{
	lua_State* L = LuaManager::instance()->getLuaMachine();
	void* data = lua_newuserdata(L, size);
	return data;
}
