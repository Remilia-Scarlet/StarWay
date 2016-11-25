#define _CRT_SECURE_NO_WARNINGS
#include <unordered_map>
#include "LuaVector.h"
#include "Vector.h"

bool LuaVector::createLuaPrototype()
{
	lua_State* L = LuaManager::instance()->getLuaMachine();
	int oldStack = lua_gettop(L);
	lua_newtable(L);

	LUA_PROTOTYPE_REGIST_FUN(__index);
	LUA_PROTOTYPE_REGIST_FUN(__newindex);
	LUA_PROTOTYPE_REGIST_FUN(__add);
	LUA_PROTOTYPE_REGIST_FUN(__sub);
	LUA_PROTOTYPE_REGIST_FUN(__mul);
	LUA_PROTOTYPE_REGIST_FUN(__div);

	LUA_PROTOTYPE_REGIST_FUN(lenth);
	LUA_PROTOTYPE_REGIST_FUN(create);

	LUA_PROTOTYPE_END(LuaVector);
	return true;
}

int LuaVector::L___index(lua_State* L)
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
		int type = lua_getglobal(L, TO_STRING(LuaVector));
		TinyAssert(type == LUA_TTABLE);
		lua_pushnil(L);
		lua_copy(L, -3, -1);
		type = lua_rawget(L, -2);
		lua_replace(L, -2);
		TinyAssert(oldStack + 1 == lua_gettop(L));
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

int LuaVector::L___newindex(lua_State* L)
{
	int oldStack = lua_gettop(L);

	if (lua_type(L, 1) != LUA_TUSERDATA)
		return LUA_PARAM_ERROR(LuaVector::L___newindex);

	if (lua_type(L, 2) != LUA_TSTRING)
		return LUA_PARAM_ERROR(LuaVector::L___newindex);

	if (lua_type(L, 3) != LUA_TNUMBER)
		return LUA_PARAM_ERROR(LuaVector::L___newindex);

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
		return LUA_PARAM_ERROR(LuaVector::L___newindex);

	float val = (float)lua_tonumber(L, 3);
	((float*)data)[keyIndex] = val;
	TinyAssert(oldStack == lua_gettop(L));
	return 0;
}

int LuaVector::L___add(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop != 2)
		return LUA_PARAM_ERROR(VectorStorage::L___add);
	if (lua_type(L, 1) != LUA_TUSERDATA || lua_type(L, 2) != LUA_TUSERDATA)
		return LUA_PARAM_ERROR(VectorStorage::L___add);
	size_t size = lua_rawlen(L, 1);
	if (size != lua_rawlen(L, 2))
		return LUA_PARAM_ERROR(VectorStorage::L___add);
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
		return LUA_PARAM_ERROR(VectorStorage::L___add);
	}
	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

int LuaVector::L___sub(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop != 2)
		return LUA_PARAM_ERROR(VectorStorage::L___sub);
	if (lua_type(L, 1) != LUA_TUSERDATA || lua_type(L, 2) != LUA_TUSERDATA)
		return LUA_PARAM_ERROR(VectorStorage::L___sub);
	size_t size = lua_rawlen(L, 1);
	if (size != lua_rawlen(L, 2))
		return LUA_PARAM_ERROR(VectorStorage::L___sub);
	switch (size)
	{
	case sizeof(Vector2) :
	{
		*(Vector2*)allocAndPushUserData(sizeof(Vector2)) = *(Vector2*)lua_touserdata(L, 1) - *(Vector2*)lua_touserdata(L, 2);
		break;
	}
	case sizeof(Vector3) :
	{
		*(Vector3*)allocAndPushUserData(sizeof(Vector3)) = *(Vector3*)lua_touserdata(L, 1) - *(Vector3*)lua_touserdata(L, 2);
		break;
	}
	case sizeof(Vector4) :
	{
		*(Vector4*)allocAndPushUserData(sizeof(Vector4)) = *(Vector4*)lua_touserdata(L, 1) - *(Vector4*)lua_touserdata(L, 2);
		break;
	}
	default:
		return LUA_PARAM_ERROR(VectorStorage::L___sub);
	}
	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

int LuaVector::L___mul(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop != 2)
		return LUA_PARAM_ERROR(VectorStorage::L___mul);
	int vecIndex = 0;
	int floatIndex = 0;
	if (lua_type(L, 1) == LUA_TUSERDATA && lua_type(L, 2) == LUA_TNUMBER)
	{
		vecIndex = 1;
		floatIndex = 2;
	}
	else if (lua_type(L, 1) == LUA_TNUMBER && lua_type(L, 2) == LUA_TUSERDATA)
	{
		vecIndex = 2;
		floatIndex = 1;
	}
	else if (lua_type(L, 1) == LUA_TUSERDATA && lua_type(L, 2) == LUA_TUSERDATA)
	{
		vecIndex = 1;
		floatIndex = -2;
	}
	else
		return LUA_PARAM_ERROR(VectorStorage::L___mul);

	size_t size = lua_rawlen(L, vecIndex);
	switch (size)
	{
	case sizeof(Vector2) :
	{
		if(floatIndex > 0)
			*(Vector2*)allocAndPushUserData(sizeof(Vector2)) = *(Vector2*)lua_touserdata(L, vecIndex) * float(lua_tonumber(L, floatIndex));
		else
			*(Vector2*)allocAndPushUserData(sizeof(Vector2)) = *(Vector2*)lua_touserdata(L, vecIndex) * *(Vector2*)lua_touserdata(L, -floatIndex);
		break;
	}
	case sizeof(Vector3) :
	{
		if (floatIndex > 0)
			*(Vector3*)allocAndPushUserData(sizeof(Vector3)) = *(Vector3*)lua_touserdata(L, vecIndex) * float(lua_tonumber(L, floatIndex));
		else
			*(Vector3*)allocAndPushUserData(sizeof(Vector3)) = *(Vector3*)lua_touserdata(L, vecIndex) * *(Vector3*)lua_touserdata(L, -floatIndex);
		break;
	}
	case sizeof(Vector4) :
	{
		if (floatIndex > 0)
			*(Vector4*)allocAndPushUserData(sizeof(Vector4)) = *(Vector4*)lua_touserdata(L, vecIndex) * float(lua_tonumber(L, floatIndex));
		else
			*(Vector4*)allocAndPushUserData(sizeof(Vector4)) = *(Vector4*)lua_touserdata(L, vecIndex) * *(Vector4*)lua_touserdata(L, -floatIndex);
		break;
	}
	default:
		return LUA_PARAM_ERROR(VectorStorage::L___mul);
	}
	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

int LuaVector::L___div(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop != 2)
		return LUA_PARAM_ERROR(VectorStorage::L___div);

	if (lua_type(L, 1) != LUA_TUSERDATA || lua_type(L, 2) != LUA_TNUMBER)
		return LUA_PARAM_ERROR(VectorStorage::L___div);

	size_t size = lua_rawlen(L, 1);
	switch (size)
	{
	case sizeof(Vector2) :
	{
		*(Vector2*)allocAndPushUserData(sizeof(Vector2)) = *(Vector2*)lua_touserdata(L, 1) / float(lua_tonumber(L, 2));
		break;
	}
	case sizeof(Vector3) :
	{
		*(Vector3*)allocAndPushUserData(sizeof(Vector3)) = *(Vector3*)lua_touserdata(L, 1) / float(lua_tonumber(L, 2));
		break;
	}
	case sizeof(Vector4) :
	{
		*(Vector4*)allocAndPushUserData(sizeof(Vector4)) = *(Vector4*)lua_touserdata(L, 1) / float(lua_tonumber(L, 2));
		break;
	}
	default:
		return LUA_PARAM_ERROR(VectorStorage::L___div);
	}
	TinyAssert(oldTop + 1 == lua_gettop(L));
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

	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

int LuaVector::L_lenth(lua_State* L)
{
	int oldTop = lua_gettop(L);
	if (oldTop < 1)
		return LUA_PARAM_ERROR(LuaVector::L_lenth);

	size_t size = lua_rawlen(L, 1);
	switch (size)
	{
	case sizeof(Vector2) :
	{
		float len = ((Vector2*)lua_touserdata(L, 1))->lenth();
		lua_pushnumber(L, len);
		break;
	}
	case sizeof(Vector3) :
	{
		float len = ((Vector3*)lua_touserdata(L, 1))->lenth();
		lua_pushnumber(L, len);
		break;
	}
	case sizeof(Vector4) :
	{
		float len = ((Vector4*)lua_touserdata(L, 1))->lenth();
		lua_pushnumber(L, len);
		break;
	}
	default:
		return LUA_PARAM_ERROR(VectorStorage::L_lenth);
	}

	TinyAssert(oldTop + 1 == lua_gettop(L));
	return 1;
}

void* LuaVector::allocAndPushUserData(int size,int metaIndex)
{
	lua_State* L = LuaManager::instance()->getLuaMachine();
	int oldTop = lua_gettop(L);
	int absIndex = lua_absindex(L, metaIndex);
	void* data = lua_newuserdata(L, size);
	lua_pushnil(L);
	lua_copy(L, absIndex, -1);
	lua_setmetatable(L, -2);
	TinyAssert(oldTop + 1 == lua_gettop(L));
	return data;
}

void* LuaVector::allocAndPushUserData(int size)
{
	lua_State* L = LuaManager::instance()->getLuaMachine();
	int oldTop = lua_gettop(L);
	int type = lua_getglobal(L, TO_STRING(LuaVector));
	TinyAssert(type == LUA_TTABLE);
	void* data = allocAndPushUserData(size, -1);
	lua_replace(L, -2);
	TinyAssert(oldTop + 1 == lua_gettop(L));
	return data;
}
