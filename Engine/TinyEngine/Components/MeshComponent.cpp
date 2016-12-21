#include "TinyEngine\precomp.h"
#include "MeshComponent.h"
#include "Graphic\Vertex\DefaultVertex.h"
#include "Graphic\Vertex\InputLayoutDefine.h"
#include "Graphic\Manager\DefaultMgr.h"
MeshComponent::MeshComponent()
	:BaseComponent(TO_STRING(MeshComponent))
{
}


bool MeshComponent::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);
	LUA_PROTOTYPE_REGIST_FUN(setPrimitiveTopology);

	LUA_PROTOTYPE_REGIST_CONST_VAL(TO_STRING(POINT_LIST), (int)PrimitiveTopology::POINT_LIST);
	LUA_PROTOTYPE_REGIST_CONST_VAL(TO_STRING(LINE_LIST), (int)PrimitiveTopology::LINE_LIST);
	LUA_PROTOTYPE_REGIST_CONST_VAL(TO_STRING(LINE_STRIP), (int)PrimitiveTopology::LINE_STRIP);
	LUA_PROTOTYPE_REGIST_CONST_VAL(TO_STRING(TRIANGLE_LIST), (int)PrimitiveTopology::TRIANGLE_LIST);
	LUA_PROTOTYPE_REGIST_CONST_VAL(TO_STRING(TRIANGLE_STRIP), (int)PrimitiveTopology::TRIANGLE_STRIP);


	LUA_PROTOTYPE_END(MeshComponent);
	return true;
}

MeshComponent::~MeshComponent()
{
}

int MeshComponent::L_create(lua_State* L)
{
	int top = lua_gettop(L);
	if (top != 3 && top != 4)
		return LUA_PARAM_ERROR(MeshComponent::L_create);

	if(lua_type(L,2) != LUA_TTABLE)
		return LUA_PARAM_ERROR(MeshComponent::L_create);

	std::vector<CommonVertex> vertexArr;
	lua_Integer arrLen = luaL_len(L, 2);
	vertexArr.resize(arrLen);
	for (lua_Integer i = 1; i <= arrLen; ++i)
	{
		CommonVertex& ver = vertexArr[i - 1];
		int type = lua_geti(L, 2, i);//2:vertex array -1:one vertex
		TinyAssert(type == LUA_TTABLE);
		type = lua_geti(L, -1, 1);//2:vertex array -2:one vertex -1:pos
		TinyAssert(type == LUA_TUSERDATA);
		ver.pos = LuaManager::instance()->getVal<Vector3>(L, -1);
		lua_pop(L, 1);//2:vertex array -1:one vertex
		type = lua_geti(L, -1, 2);//2:vertex array -2:one vertex -1:normal
		TinyAssert(type == LUA_TUSERDATA);
		ver.normal = LuaManager::instance()->getVal<Vector3>(L, -1);
		lua_pop(L, 1);//2:vertex array -1:one vertex
		type = lua_geti(L, -1, 3);//2:vertex array -2:one vertex -1:uv
		TinyAssert(type == LUA_TUSERDATA);
		ver.uv = LuaManager::instance()->getVal<Vector2>(L, -1);
		lua_pop(L, 1);//2:vertex array -1:one vertex

		lua_pop(L, 1);//2:vertex array
	}
	if (lua_type(L,3) == LUA_TSTRING)
	{
		MeshComponentPtr mesh = MeshComponent::create(vertexArr, lua_tostring(L, 3));
		if (!mesh.isValid())
		{
			return luaL_error(L, "create mesh failed");
		}
		LuaManager::instance()->pushVal(L, mesh);
		return 1;
	}
	else if (lua_type(L,3) == LUA_TTABLE && lua_type(L,4) == LUA_TSTRING)
	{
		std::vector<int> ind;
		lua_Integer indsLen = luaL_len(L, 3);
		ind.resize(indsLen);
		for (lua_Integer i = 1; i <= indsLen; ++i)
		{
			int type = lua_geti(L, 3, i);
			TinyAssert(type == LUA_TNUMBER);
			ind[i - 1] = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
		MeshComponentPtr mesh = MeshComponent::create(vertexArr, ind, lua_tostring(L, 4));
		LuaManager::instance()->pushVal(L, mesh);
		return 1;
	}
	else
		return LUA_PARAM_ERROR(MeshComponent::L_create);
}

void MeshComponent::setPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
	_gfxMesh->setPrimitiveTopology(primitiveTopology);
}

int MeshComponent::L_setPrimitiveTopology(lua_State* L)
{
	ASSERT_PARAM_NUM(2);
	MeshComponentPtr self = LuaManager::instance()->getVal<MeshComponentPtr>(L, 1);
	PrimitiveTopology topology = (PrimitiveTopology)LuaManager::instance()->getVal<int>(L, 2);
	if (!self.isValid())
		return LUA_PARAM_ERROR(MeshComponent::L_setPrimitiveTopology);

	self->setPrimitiveTopology(topology);
	return 0;
}

PrimitiveTopology MeshComponent::getPrimitiveTopology() const
{
	return _gfxMesh->getPrimitiveTopology();
}

void MeshComponent::render()
{
	_gfxMesh->render(_vertexShader);
	if (_vertexShader.isValid())
		_vertexShader->render();
	else
		DefaultMgr::instance()->getDefaultVS()->render();
}
