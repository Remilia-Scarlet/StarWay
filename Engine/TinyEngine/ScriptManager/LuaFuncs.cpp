#include "TinyEngine/precomp.h"
#include "LuaFuncs.h"
#include "LuaManager.h"
#include <functional>
#include "TinyEngine/Scene/Scene.h"
#include "TinyEngine/Components/MeshComponent.h"
#include "TinyEngine/Other/GeometryGenerator.h"
#include "Graphic/Vertex/InputLayoutDefine.h"
#include "TinyEngine/Components/TextureComponent.h"
#include "TinyEngine/Components/TransformComponent.h"
#include "TinyEngine/Components/CameraComponent.h"
#include "TinyEngine/Engine/Engine.h"
#include "TinyEngine/Components/ScriptComponent.h"
#include "TinyEngine/Components/DirectionLightComponent.h"
#include "TinyEngine/Input/InputManager.h"

LuaFuns* LuaFuns::instance()
{
	static LuaFuns instance;
	return &instance;
}

int LuaFuns::GenerateCubeMesh(lua_State* L)
{
	LuaVal w = getVal(L, 1);
	LuaVal h = getVal(L, 2);
	LuaVal l = getVal(L, 3);
	if (!w.isNumber() || !h.isNumber() || !l.isNumber())
		return LUA_PARAM_ERROR(GenerateCubeMesh);
	MeshComponentPtr mesh = GeometryGenerator::instance()->createCubeMesh(InputLayoutType::COMMON, w.convertFloat(), h.convertFloat(), l.convertFloat());
	pushVal(L, mesh);
	return 1;
}

int LuaFuns::GenerateSphereMesh(lua_State* L)
{
	LuaVal radius = getVal(L, 1);
	LuaVal numSubdivisions = getVal(L, 2);
	if (!radius.isNumber() || !numSubdivisions.isNumber())
		return LUA_PARAM_ERROR(GenerateCubeMesh);
	MeshComponentPtr mesh = GeometryGenerator::instance()->createSphereMeshData(InputLayoutType::COMMON, radius.convertFloat(), numSubdivisions.convertInt32());
	pushVal(L, mesh);
	return 1;
}

int LuaFuns::StartScene(lua_State* L)
{
	ScenePtr scene = getVal(L, 1).convertRefPtr_dynamic<Scene>();
	if(!scene.isValid())
		return LUA_PARAM_ERROR(StartScene);
	Engine::instance()->startScene(scene);
	return 0;
}

int LuaFuns::GetCurrentScene(lua_State* L)
{
	ScenePtr scene = Engine::instance()->getCurrentScene();
	pushVal(L, scene);
	return 1;
}

bool LuaFuns::registerFuncsToLua()
{
	// object
	Object::createLuaPrototype();
	Scene::createLuaPrototype();
	
	// component
	CameraComponent::createLuaPrototype();
	DirectionLightComponent::createLuaPrototype();
	MeshComponent::createLuaPrototype();
	ScriptComponent::createLuaPrototype();
	TextureComponent::createLuaPrototype();
	TransformComponent::createLuaPrototype();

	// singleton manager
	InputManager::createLuaPrototype();

	// global function
#define REGIST_FUN(_NAME_) do{lua_register(LuaManager::instance()->getLuaMachine(), #_NAME_, LuaFuns::_NAME_);}while(0)
	REGIST_FUN(GenerateCubeMesh);
	REGIST_FUN(GenerateSphereMesh);
	REGIST_FUN(StartScene);
	REGIST_FUN(GetCurrentScene);
#undef REGIST_FUN

	return true;
}

bool LuaFuns::pushVal(lua_State * L, const LuaVal& val)
{
	return LuaManager::instance()->pushVal(L, val);
}

LuaVal LuaFuns::getVal(lua_State * L, int index)
{
	return LuaManager::instance()->getVal(L, index);
}
