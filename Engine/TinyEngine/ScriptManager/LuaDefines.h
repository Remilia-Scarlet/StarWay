#pragma once

#define LUA_PARAM_ERROR(NAME) luaL_error(L, "function %s param error", #NAME)
static const char* CPP_LUA_POTABLE = "_ObjList";
static const char* LUA_CPP_REF_NAME = "c_ref";
static const char* CPP_CALL_LUA_NAME = "CppCallLua";
static const char* COMPONENT_PARENT = "parent";
static const char* SCRIPT_COMPONENT_TABLE_OBJ = "obj";

// Used in prototype creating. First PROTOTYPE_PREPARE() then PROTOTYPE_REGISTER_FUN(FUN_NAME) if needed. Then PROTOTYPE_END(CLASS_NAME).
#define LUA_PROTOTYPE_PREPARE()       \
	lua_State* L = LuaManager::instance()->getLuaMachine();\
	int oldStack = lua_gettop(L); \
	lua_newtable(L);              \
	lua_pushstring(L, "__index"); \
	lua_pushnil(L);               \
	lua_copy(L, -3, -1);          \
	lua_rawset(L, -3);

#define LUA_PROTOTYPE_REGIST_FUN(FUN_NAME) do {lua_pushstring(L, #FUN_NAME);lua_pushcfunction(L, L_##FUN_NAME);lua_rawset(L, -3);} while (0)

#define LUA_PROTOTYPE_REGIST_CONST_VAL(NAME,VALUE) do{LuaManager::instance()->pushVal(L, NAME);LuaManager::instance()->pushVal(L, VALUE);lua_rawset(L, -3);}while(0)

#define LUA_PROTOTYPE_END(CLASS_NAME)\
	lua_setglobal(L, #CLASS_NAME);\
	TinyAssert(lua_gettop(L) == oldStack);

// create a lua object with prototype CLASS_NAME onto the top of stack
#define LUA_GENERATE_OBJ(CLASS_NAME)\
	do \
	{\
		lua_State* L = LuaManager::instance()->getLuaMachine();\
		int oldSize = lua_gettop(L);\
		lua_getglobal(L, CPP_LUA_POTABLE);\
		lua_pushinteger(L, lua_Integer(this));\
		lua_newtable(L);\
		lua_getglobal(L, CLASS_NAME);\
		lua_setmetatable(L, -2);\
		lua_pushstring(L, LUA_CPP_REF_NAME);\
		lua_pushlightuserdata(L, this);\
		lua_rawset(L, -3);\
		lua_rawset(L, -3);\
		lua_pop(L, 1);\
		TinyAssert(oldSize == lua_gettop(L));\
	}while(0)

#define LUA_REMOVE_OBJ()\
	do \
	{\
		if (Engine::instance()->isExiting())\
			break; \
		lua_State* L = LuaManager::instance()->getLuaMachine(); \
		int oldSize = lua_gettop(L); \
		lua_getglobal(L, CPP_LUA_POTABLE); \
		lua_pushnil(L); \
		lua_rawseti(L, -2, lua_Integer(this)); \
		lua_pop(L, 1); \
		TinyAssert(oldSize == lua_gettop(L));\
	} while (0)


#define LUA_CREATE_FUN_P0(CLASS_NAME)\
	static int L_create(lua_State* L)\
	{\
		LuaManager::instance()->pushVal(CLASS_NAME##::create());\
		return 1;\
	}

#define LUA_CREATE_FUN_P1(CLASS_NAME,P1_TYPE)\
	static int L_create(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::create(p1));\
		return 1;\
	}

#define LUA_CREATE_FUN_P2(CLASS_NAME,P1_TYPE,P2_TYPE)\
	static int L_create(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::create(p1,p2));\
		return 1;\
	}

#define LUA_CREATE_FUN_P3(CLASS_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	static int L_create(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P3_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::create(p1,p2,p3));\
		return 1;\
	}

#define LUA_CREATE_FUN_P4(CLASS_NAME,P1_TYPE,P2_TYPE,P3_TYPE,P4_TYPE)\
	static int L_create(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P3_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		P4_TYPE p4 = LuaManager::instance()->getVal(L,5).convert<P4_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::create(p1,p2,p3,p4));\
		return 1;\
	}

#define LUA_MEMBER_FUN_P0R0_DECL(CLASS_NAME,FUN_NAME)\
	static int L_##FUN_NAME##(lua_State* L)

#define LUA_MEMBER_FUN_P0R0_IMPL(CLASS_NAME,FUN_NAME)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		if(self)\
			self->##FUN_NAME();\
		return 0;\
	}

#define LUA_MEMBER_FUN_P1R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	static int L_##FUN_NAME##(lua_State* L);

#define LUA_MEMBER_FUN_P1R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		if(self)\
			self->##FUN_NAME(p1);\
		return 0;\
	}

#define LUA_MEMBER_FUN_P2R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	static int L_##FUN_NAME##(lua_State* L);

#define LUA_MEMBER_FUN_P2R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		if(self)\
			self->##FUN_NAME(p1,p2);\
		return 0;\
	}

#define LUA_MEMBER_FUN_P3R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	static int L_##FUN_NAME##(lua_State* L)

#define LUA_MEMBER_FUN_P3R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P3_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		if(self)\
			self->##FUN_NAME(p1,p2,p3);\
		return 0;\
	}

#define LUA_MEMBER_FUN_P0R1_DECL(CLASS_NAME,FUN_NAME)\
	static int L_##FUN_NAME##(lua_State* L)

#define LUA_MEMBER_FUN_P0R1_IMPL(CLASS_NAME,FUN_NAME)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		if(self)\
			LuaManager::instance()->pushVal(self->##FUN_NAME());\
		return 1;\
	}

#define LUA_MEMBER_FUN_P1R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	static int L_##FUN_NAME##(lua_State* L)

#define LUA_MEMBER_FUN_P1R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		if(self)\
			LuaManager::instance()->pushVal(self->##FUN_NAME(p1));\
		return 1;\
	}

#define LUA_MEMBER_FUN_P2R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	static int L_##FUN_NAME(lua_State* L)\

#define LUA_MEMBER_FUN_P2R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		if(self)\
			LuaManager::instance()->pushVal(self->##FUN_NAME(p1,p2));\
		return 1;\
	}

#define LUA_MEMBER_FUN_P3R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	static int L_##FUN_NAME##(lua_State* L)\

#define LUA_MEMBER_FUN_P3R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P3_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		if(self)\
			LuaManager::instance()->pushVal(self->##FUN_NAME(p1,p2,p3));\
		return 1;\
	}

#define LUA_SINGLETON_FUN_P0R0_DECL(CLASS_NAME,FUN_NAME)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_SINGLETON_FUN_P0R0_IMPL(CLASS_NAME,FUN_NAME)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		CLASS_NAME##::instance()->##FUN_NAME();\
		return 0;\
	}

#define LUA_SINGLETON_FUN_P1R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_SINGLETON_FUN_P1R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		CLASS_NAME##::instance()->##FUN_NAME(p1);\
		return 0;\
	}

#define LUA_SINGLETON_FUN_P2R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_SINGLETON_FUN_P2R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		CLASS_NAME##::instance()->##FUN_NAME(p1,p2);\
		return 0;\
	}

#define LUA_SINGLETON_FUN_P3R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_SINGLETON_FUN_P3R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P3_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		CLASS_NAME##::instance()->##FUN_NAME(p1,p2,p3);\
		return 0;\
	}


#define LUA_SINGLETON_FUN_P0R1_DECL(CLASS_NAME,FUN_NAME)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_SINGLETON_FUN_P0R1_IMPL(CLASS_NAME,FUN_NAME)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		LuaManager::instance()->pushVal(CLASS_NAME##::instance()->##FUN_NAME());\
		return 1;\
	}

#define LUA_SINGLETON_FUN_P1R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_SINGLETON_FUN_P1R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::instance()->##FUN_NAME(p1));\
		return 1;\
	}

#define LUA_SINGLETON_FUN_P2R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_SINGLETON_FUN_P2R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::instance()->##FUN_NAME(p1,p2));\
		return 1;\
	}

#define LUA_SINGLETON_FUN_P3R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_SINGLETON_FUN_P3R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME##(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P2_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P3_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::instance()->##FUN_NAME(p1,p2,p3));\
		return 1;\
	}

