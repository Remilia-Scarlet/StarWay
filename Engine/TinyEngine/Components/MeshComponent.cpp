#include "TinyEngine\precomp.h"
#include "MeshComponent.h"
#include "Graphic\Vertex\CommonVertex.h"
#include "Graphic\Vertex\InputLayoutDefine.h"
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

	LuaVal vertexs = LuaManager::instance()->getVal(L, 2);
	if(vertexs.getType() != LuaVal::DataType::TABLE)
		return LUA_PARAM_ERROR(MeshComponent::L_create);
	std::vector<CommonVertex> ver;
	for (int i = 1; i <= vertexs.getLenth(); ++i)
	{
		LuaVal vertex = vertexs.getField(i);
		Vector3 pos = (Vector3)vertex.getField(1);
		Vector3 normal = (Vector3)vertex.getField(2);
		Vector2 uv = (Vector2)vertex.getField(3);

		ver.push_back({ pos,normal,uv });
	}

	LuaVal indicsOrName = LuaManager::instance()->getVal(L, 3);
	if (indicsOrName.getType() == LuaVal::DataType::STRING)
	{
		MeshComponentPtr mesh = MeshComponent::create(InputLayoutType::COMMON, ver, indicsOrName.convertCharPointer());
		if (!mesh.isValid())
		{
			return luaL_error(L, "create mesh failed");
		}
		LuaManager::instance()->pushVal(L, mesh);
		return 1;
	}
	else if (indicsOrName.getType() == LuaVal::DataType::TABLE)
	{
		std::vector<int> ind;
		for (int i = 1; i <= indicsOrName.getLenth(); ++i)
		{
			ind.push_back(indicsOrName.getField(i).convertInt32());
		}
		MeshComponentPtr mesh = MeshComponent::create(InputLayoutType::COMMON, ver, ind, LuaManager::instance()->getVal(L, 4).convertCharPointer());
		LuaManager::instance()->pushVal(L, mesh);
		return 1;
	}
	else
		return LUA_PARAM_ERROR(MeshComponent::L_create);
}

void MeshComponent::setMaterial(const Material& material)
{
	_gfxMesh->setMaterial(material);
}

const Material& MeshComponent::getMaterial()
{
	return _gfxMesh->getMaterial();
}

void MeshComponent::setPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
	_gfxMesh->setPrimitiveTopology(primitiveTopology);
}

int MeshComponent::L_setPrimitiveTopology(lua_State* L)
{
	ASSERT_PARAM_NUM(2);
	MeshComponentPtr self = LuaManager::instance()->getVal(L, 1).convertRefPtr_dynamic<MeshComponent>();
	PrimitiveTopology topology = (PrimitiveTopology)LuaManager::instance()->getVal(L, 2).convertInt32();
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
	_vertexShader->render();
}
